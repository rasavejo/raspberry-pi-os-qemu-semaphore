#include "sem.h"
#include "sched.h"
#include "mm.h"
#include "irq.h"

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

void sem_delete(semaphore sem) {
    disable_irq();
    SEM(sem) = 0;
    enable_irq();
}

void block_fut(semaphore sem) {
    disable_irq();
    current->state = TASK_BLOCKED;
    current->blocked_by = sem;
    enable_irq();
    schedule();
}

void sem_p(semaphore sem) {
    if (SEM(sem) >> 16) {
        disable_irq();
        SEM(sem) -= 1 << 16;
        enable_irq();
    }
    else
        block_fut(sem);
}

void sem_v(semaphore sem) {
    disable_irq();
    SEM(sem) += 1 << 16;
    enable_irq();
}
