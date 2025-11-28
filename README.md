# Implementing semaphore in a bare-metal Aarch64 environnment for RaspberryPi 3 !

Adding semaphores to the mini-OS previously used to learn how OSes work.

## Goal 

## V1 : Using syscalls

### Semaphores operations

The semaphores are base on two main operations : P and V.
 - The P operation allows a process to take a token from the semaphore. If a token is available, the process can immediatly take it and enter the critical section. In the opposite, if there is no token available, the process will enters a blocked state until a new token is released. 
 - Once a process wants to leave the critical section, it can call the V operation which will release the token. 

In addition to the P and V operations, we also need to implement 2 more functions to create and delete semaphores. 

[side note] In the user process, the semaphores needs to be initialized before the creation of the childs process in order for them to access it. 

We decided to implement all of those new operations in a new [sem.c](/src/lesson06/src/sem.c) file. 

#### Creating a semaphore 
 ```
 
#define SEM(s) *(unsigned long*)(sem_page + s)

unsigned long sem_page;

void sem_table_init() {
    sem_page = allocate_kernel_page(); // already zeroed
}

unsigned long sem_new(unsigned int count) {
    unsigned long sem = 0;
    while ((SEM(sem)) % 2) sem++;
    SEM(sem) = count << 16;
    return sem;
    
}
 ```
#### Delecting a semaphore
 ```
 void sem_delete(unsigned long sem) {
    disable_irq();
    SEM(sem) = 0;
    enable_irq();
}
 ```
#### Taking a token
```
 void sem_p(semaphore sem) {
    if (SEM(sem) >> 16) {
        disable_irq();
        SEM(sem) -= 1 << 16;
        enable_irq();
    }
    else
        block(sem);
}
 ```

 If the semaphore does not have any token available, then the current process needs to be blocked.  
 In order to block a process, a feature as already been implemented in the scheduler to only give the CPU to ```TASK_RUNNING``` tasks. Therefore, in order to block a task, we need to create a personalized state which we decided to call ```TASK_BLOCKED```. This state is added along the other possible states in the file [sched.h](/src/lesson06/src/sched.h) :

 ```
#define TASK_RUNNING			0
#define TASK_ZOMBIE				1
#define TASK_BLOCKED			2
 ```

Finally, it is time to show how we implement the blocking operation : 
```
 void block(semaphore sem) {
    disable_irq();
    current->state = TASK_BLOCKED;
    current->blocked_by = sem;
    enable_irq();
    schedule();
}
```

One thing we did not mentioned yet was how to unblock a task once a new token is available. We had multiple options available : 
 - Using an array to memorize the tasks that are waiting for a same semaphore token. In that case, once a token is release, we would unblock the first task of the array and give it the newly released token. 
 - Each blocked task memorize which semaphore it is waiting for. We give the scheduler the role of checking weither a blocked task can be unblocked or not.  

As you can see in the code above, it is the second option that we decided to implement. This option seemed more conveniant as it only requires adding a new field to the ```task_struct```. Moreover, there are not all the problems regarding managing the size of the array and inserting, removing tasks from it.  
Moving forward with our decision, we modified the ```task_struct``` in the [sched.h](/src/lesson06/src/sched.h) file : 

```
struct task_struct {
	struct cpu_context cpu_context;
	long state;
	long counter;
	long priority;
	long preempt_count;
	unsigned long blocked_by;
	unsigned long flags;
	struct mm_struct mm;
};
```

Please take note of where we decided to add the ```blocked_by``` field : between ```preempt_count``` and ```flags```. This is important because we also need to add a default value to this field : 

```
/* state etc */	 0,0,15, 0, -1,  PF_KTHREAD, \
```

We effectively added a '-1' for the default value of ```blocked_by```, meaning that a new task is not blocked by anything yet. We can ensure that the value was placed in the correct space when we look at the flag PF_KTHREAD : the -1 is just before it, as the ```blocked_by``` field is define just before the ```flags``` field.
  
TODO MODIFY FORK.C COPY PROCESS  
  
TODO SCHEDULER MODIFIED TO UNBLOCK A TASK 

#### Releasing a token
 ```
 void sem_v(semaphore sem) {
    disable_irq();
    SEM(sem) += 1 << 16;
    enable_irq();
}
 ```
### Creating new syscalls

The different operations used to manage the semaphores will be accessible from a user environnment thought ```syscalls```.  
Consequently, 4 new syscalls needs to be added to our bare-metal environnment : 
 - Creating a new semaphore
 - Taking a semaphore token 
 - Releasing a semaphore token
 - Delecting a semaphore 

Those new syscalls are implemented along the ```sys_write```, ```sys_exit```, and ```sys_fork``` already implemented in the [sys.c](/src/lesson06/src/sys.c) file.

 ```
unsigned long sys_sem_new(unsigned int count){
	sem_new(count);
}
void sys_sem_p(semaphore sem){
	sem_p(sem);
}
void sys_sem_v(semaphore sem){
	sem_v(sem);
}
void sys_sem_delete(semaphore sem){
	sem_delete(sem);
}
 ```
As the semaphores operations can be quite tricky to implement, 

### Testing 

#### 1 task and 1 semaphore 1 token

The first test done is made of only one task and one semaphore. We first want to test weither a task can create a semaphore, take a token, enter the critical section and then release it. 

#### 2 task and 1 semaphore 1 token

The second test ensure that if a task is asking for a token and there is not token available, then this task will be blocked until the token is released. 

## V2 : Using Futex

Por 

# Acknowledgements

- Sergey Matyukevich, Alexandre Venito et al. made the educational kernel and most of the resources in https://github.com/s-matyukevich/raspberry-pi-os
- Felix Xiaozhu Lin et al. adapted the codebase to make it work on QEMU and improved the content in https://github.com/fxlin/p1-kernel
- Millian Poquet adapted the codebase for his operating system design course in https://github.com/mpoquet/raspberry-pi-os-qemu
