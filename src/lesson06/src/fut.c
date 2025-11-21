#include "fut.h"
#include "sched.h"
#include "mm.h"
#include "irq.h"

#define FUT(s) *(unsigned long*)(fut_page + s)

void fut_table_init() {
    fut_page = allocate_kernel_page(); // already zeroed
}


unsigned long get_fut_page() {
    return allocate_user_page(current,fut_page);
}

unsigned long fut_new(unsigned int count) {
    unsigned long fut = 0;
    while ((FUT(fut)) % 2) fut++;
    FUT(fut) = count << 16;
    return fut;
    
}

void fut_delete(unsigned long fut) {
    FUT(fut) = 0;
}

void block(unsigned long fut) {
    disable_irq();
    current->state = TASK_BLOCKED;
    current->blocked_by = fut;
    enable_irq();
    schedule();
}