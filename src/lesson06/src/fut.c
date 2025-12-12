#include "fut.h"
#include "futasm.h"
#include "sched.h"
#include "mm.h"
#include "irq.h"

unsigned long fut_page;

#define FUT(page,s) *(unsigned long*)(page + s)

void fut_table_init() {
    fut_page = allocate_kernel_page(); // already zeroed
    FUT(fut_page,0) = 1 << 16;
    FUT(fut_page,0) += 1;
}


unsigned long get_fut_page() {
    return allocate_user_page(current,fut_page);
}

unsigned long fut_new(unsigned long page, unsigned int count) {
    fut_p(page,0);
    unsigned long fut = 0;
    while ((FUT(page,fut)) % 2) fut++;
    FUT(page,fut) = count << 16;
    fut_v(page,0);
    return fut;
}

void fut_delete(unsigned long page, unsigned long fut) {
    FUT(page,fut) = 0;
}

void fut_block(unsigned long fut) {
    disable_irq();
    current->state = TASK_BLOCKED;
    current->blocked_by = fut;
    enable_irq();
    schedule();
}

void fut_p(unsigned long page, unsigned long fut) {
    int blocked = fut_pasm(fut, page);
    if (blocked) fut_block(fut);
}

void fut_v(unsigned long page,unsigned long fut) {
    fut_vasm(fut, page);
}