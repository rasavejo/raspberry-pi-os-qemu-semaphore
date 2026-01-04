## V2 : Futex-inspired semaphores : Futaphores

### Implementation 
Since futaphores are a reimplementation of semaphores, we will discuss their implementation before their operations, as those are entirely implementation-dependent.

Semaphores follow the same convention as semaphores in their definition, and also exist in their own memory page ; this time user allocated.

To avoid needing to implement complex memory sharing and since futaphores use a single page, its address is stored inside a global variable in the system (of course, inaccessible by the user), and can then be obtained by any user through the `get_fut_page` syscall.

The main difference between semaphores and futaphores is that futaphores require a lot less syscalls, namely one when calling `get_fut_page`, and one when a P operation is blocking.
In every other case, simple function calls, accessible by the user will suffice, reducing the overhead by a lot.

Since we do not have access to syscalls, we cannot depend on interruption blocking to make the semaphores atomic.
Therefore, we need to use lower level atomic operations. The ideal solution to our problem would have been to rely on arm's own atomic operations, especially [**Compare and Swap** (CAS)](https://developer.arm.com/documentation/ddi0596/2021-06/Base-Instructions/CASH--CASAH--CASALH--CASLH--Compare-and-Swap-halfword-in-memory-?lang=en). However, this operation (and every other atomic operation) was only added to the language in a latter armv8 version to the one on our Raspberry Pi 3s. 

Therefore, we used an implementation similar to [ARM's implementation](https://documentation-service.arm.com/static/5efa1989dbdee951c1ccdea1?token) of semaphores based on **exclusive memory operations**.

[ajouter image du code asm]

This implementation doesn't provide actual mutual exclusion, but makes it so semaphores can never be written to by multiple tasks at once. Therefore, by making their functionality dependent on this write operation (meaning : if it fails, we try again), all our operations can become functionnally mutually exclusive.

### Futaphore operations

#### fut_new and fut_delete

```c
#define FUT(page,s) *(unsigned long*)(page + s*32)

unsigned long fut_new(unsigned long page, unsigned int count){
    fut_p(page,0); 
    unsigned long fut = 0;
    while (((FUT(page, fut)) & 1) == 1) fut++;
    FUT(page,fut) = count << 16 | 1;
    fut_v(page, 0);
    return fut;
}
```

```c
void fut_delete(unsigned long page, unsigned long fut){
  FUT(page,fut) = 0;
  return;
}
```

To avoid having to use syscalls, we decided to use our own semaphores as the locking mechanism to the `fut_page` (see `fut_table_init` for its creation). 

Besides that, it function exactly like `sem_new`, same for `fut_delete`.

### fut_table_init and get_fut_page

```c
unsigned long fut_page

void fut_table_init() {
    fut_page = allocate_kernel_page(); // already zeroed
    FUT(0) = 1 << 16;
    FUT(0) += 1;
}
```

```c
unsigned long get_fut_page() {
    return allocate_user_page(current,fut_page);
}
```

In these, we create `fut_page` and populate it with a single semaphore that we use to manage the page's mutual exclusion (see previous chapter). We then define `get_fut_page`, a syscall that allocates a user page in the MMU and makes it point to the `fut_page` created previously.
The futaphores themselves follow the same architecture as our previously defined semaphores.
This page is also stored in a global variable, but because of the lack of shared memory, we used a workaround : the `fut_page` memory page is allocated in kernel space, but the `get_fut_page` syscall "allocates" a user page for it, in practice simply creating a link to it in the process. 
While this approach is both very practical and efficient, it has huge security risks.
Since the page is mapped in user memory, the user could technically write to it as they please, and by doing that bypass the locking mechanism, or even just destroy semaphores for the entire system. We decided that this flaw was too complicated to fix without introducing significant overhead (for example, accessing the page via a syscall), or would make our task nigh impossible in the imposed timeframe (making better shared memory by restricting which tasks have access to each semaphores).

#### fut_v

```c
void fut_v(unsigned long page, unsigned long fut) {
  fut_vasm(fut, page);
  return;
}
```

```asm
.globl fut_vasm
fut_vasm :
	mov x3,#32
	mul x0,x0,x3
    add x1,x0,x1
fut_v_try :
    LDXR x0,[x1]
    ADD x0, x0, 0b10000000000000000     // 1 shift 16
    STXR w2,x0,[x1]
    CMP w2,#0
    BNE fut_v_try
    
    ret
```

Since the C code only calls the assembly, we will focus only on it.

```asm
	mov x3,#32
	mul x0,x0,x3
    add x1,x0,x1
```
First of all, we compute the address of our semaphore.

```asm
fut_v_try :
    LDXR x0,[x1]
    ADD x0, x0, 0b10000000000000000     // 1 shift 16
    STXR w2,x0,[x1]
    CMP w2,#0
    BNE fut_v_try
```
Then, we can use our locking mechanism : 
`LDXR x0,[x1]` first of all, we *exclusively* load our semaphore (this will set a flag that will allow a single exclusive store, and then go back to normal)
`ADD x0, x0, 0b10000000000000000` then, we add one to our semaphore (that is offset by 16 bits to the left)
`STXR w2,x0,[x1]` and finally, we write back our new value at the semaphore's address. This operation will fail if another program wrote at this address already, making our program functionnally unable to write if its load is outdated.
`CMP w2,#0    BNE fut_v_try` if the store operation failed, we just try again, until it works.

This program does not provide actual mutual exclusion, however knowing that we cannot write if the value we know is outdated will have the exact same results, and even if two programs "cross" eachother, the exclusivity flag will have been reset, the store will fail and it will run again, with the correct value.
This approach is inspired by [ARM's implementation](https://documentation-service.arm.com/static/5efa1989dbdee951c1ccdea1?token) of semaphores. 
While we couldn't test this method on a multi-threaded system, its implementation should be robust enough to permit synchronization without problem.
One very glaring "flaw" is the active wait loop used if the semaphore is already taken, however, in a normal context it is quite rare that this many processes try to give back a token at once, leading to very rare occurences of it looping even thrice.

#### fut_v

```c
void fut_p(unsigned long page, unsigned long fut) {
    int blocked = fut_pasm(fut, page);
    if (blocked == 1) {
      call_sys_fut_block(fut);
    }
    return;
}
```
```asm
.globl fut_pasm
fut_pasm :
	mov x3,#32
	mul x0,x0,x3
    add x1,x0,x1
fut_p_try :
    LDXR x0,[x1]
	mov x2,x0,lsr #16
    CMP x2,#0
    BNE fut_p_true
    MOV x0, #1  // blocking
    RET
fut_p_true : 
    SUB x0, x0, 0b10000000000000000  // 1 shift 16
    STXR w2,x0,[x1]
    CMP w2,#0
    BNE fut_p_try
    mov x0,#0  // not blocking
    RET
```

Here, the C code isn't completely transparent. While the `fut_p` is not a syscall itself, it does need to call a syscall to be able to block a task (as blocking is done in the scheduler, and it would be way too unsafe to give the user access to the scheduler). However, to enhance performance, this syscall is only called when absolutely necessary, and therefore all the semaphore logic is ran in userspace. To do this, we use a very similar assembly program than in `fut_v`, with a few differences :
```asm
fut_p_try :
    LDXR x0,[x1]
	mov x2,x0,lsr #16
    CMP x2,#0
    BNE fut_p_true
    MOV x0, #1  // blocking
    RET
fut_p_true : 
    SUB x0, x0, 0b10000000000000000  // 1 shift 16
    STXR w2,x0,[x1]
    CMP w2,#0
    BNE fut_p_try
    mov x0,#0  // not blocking
    RET
```
We added, when loading the semaphore, a check to verify its state, and when the semaphore has no tokens left, we can simply exit the function with the correct exit code. We do not need to clean the exclusive flag on the loaded semaphore, as a LDXR is not blocking to another LDXR.
If the semaphore isn't locked however, we can simply proceed as we would with a v operation, save replacing the `ADD` with a `SUB`.

blocking is handled by the `fut_block` syscall, that works in the exact same way as `sem_block`.
```c
void fut_block(unsigned long fut) {
    disable_irq();
    current->state = TASK_BLOCKED_FUT;
    current->blocked_by = fut;
    enable_irq();
    schedule();
}
```


