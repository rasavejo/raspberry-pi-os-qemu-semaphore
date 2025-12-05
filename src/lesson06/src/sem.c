#include "sem.h"
#include "sched.h"
#include "mm.h"
#include "irq.h"
#include "printf.h"

// represents the content of the semaphore number s
#define SEM(s) * (unsigned long*) ( sem_page + s*32)

unsigned long sem_page;

void sem_table_init() {
    sem_page = allocate_kernel_page(); // already zeroed
}

unsigned long sem_new(unsigned int count) {
    disable_irq();
    unsigned long sem = 0;
    while ((SEM(sem)) % 2) {
        sem++;
    }
    SEM(sem) = count << 16 | 1 ;
    enable_irq(); 
    return sem;
}

unsigned int sem_count(unsigned long sem) {
    disable_irq();
    return SEM(sem) >> 16;
    enable_irq();
}

unsigned int sem_count(unsigned long sem) {
    disable_irq();
    return SEM(sem) >> 16;
    enable_irq();
}

void sem_delete(unsigned long sem) {
    disable_irq();
    SEM(sem) = 0;
    enable_irq();
}

void block(unsigned long sem) {
    disable_irq();
    current->state = TASK_BLOCKED;
    current->blocked_by = sem;
    enable_irq();
    schedule();
}

void sem_p(unsigned long sem) {
    if (SEM(sem) >> 16) {
        disable_irq();
        SEM(sem) -= 1 << 16;
        enable_irq();
    }
    else
        block(sem);
}

void sem_v(unsigned long sem) {
    disable_irq();
    SEM(sem) += 1 << 16;
    enable_irq();
}
