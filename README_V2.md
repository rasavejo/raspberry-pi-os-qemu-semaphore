## V2 : Futex-inspired semaphores : Futaphores

### Implementation 
Since futaphores are a reimplementation of semaphores, we will discuss their implementation before their operations, as those are entirely implementation-dependent.

Semaphores follow the same convention as semaphores in their definition, and also exist in their own memory page ; this time user allocated.

To avoid needing to implement complex memory sharing and since futaphores use a single page, its address is stored inside a global variable in the system (of course, inaccessible by the user), and can then be obtained by any user through the `get_fut_page` syscall.

The main difference between semaphores and futaphores is that futaphores require a lot less syscalls, namely one when calling `get_fut_page`, and one when a P operation is blocking.

Since we do not have access to syscalls, we cannot depend on interruption blocking to make the semaphores atomic.
Therefore, we need to use lower level atomic operations. The ideal solution to our problem would have been to rely on arm's own atomic operations, especially [**Compare and Swap** (CAS)](https://developer.arm.com/documentation/ddi0596/2021-06/Base-Instructions/CASH--CASAH--CASALH--CASLH--Compare-and-Swap-halfword-in-memory-?lang=en). However, this operation (and every other atomic operation) was only added to the language in a latter version to the one on our Raspberry Pi 3s. 

Therefore, we used [ARM's implementation](https://documentation-service.arm.com/static/5efa1989dbdee951c1ccdea1?token) of semaphores based on **exclusive memory operations**.

[ajouter image du code asm]

This implementation doesn't provide actual mutual exclusion, but makes it so semaphores can never be written to by multiple tasks at once. Therefore, by making their functionment dependent on this write operation (meaning : if it fails, we try again), all our operations can become functionnally mutually exclusive.

### Futaphore operations

#### fut_new

```c
unsigned long fut_new(unsigned long page, unsigned int count) {
    fut_p(page, 0);
    unsigned long fut = 0;
    while ((FUT(page, fut)) % 2) fut++;
    FUT(page, fut) = count << 16;
    fut_v(page, 0);
    return fut;
}
```
To avoid having to use syscalls, we decided to use our own semaphores as the locking mechanism to the fut_page (see `fut_table_init` for its creation). 

Besides that, it function exactly like sem_new.

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

In these, we create `fut_page` and populate it with a single semaphores, used to manage its own mutual exclusion (see previous chapter). We then define `get_fut_page`, a syscall that allocates a user page in the MMU and makes it point to the `fut_page` created previously.
