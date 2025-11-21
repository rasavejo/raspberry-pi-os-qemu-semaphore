# Implementing semaphore in a bare-metal Aarch64 environnment for RaspberryPi 3 !

Adding semaphores to the teaching mini-OS previously used in learning how OSes work.

## Goal 

## V1 : Using syscalll

### Semaphores operations

### Creating new syscalls

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
