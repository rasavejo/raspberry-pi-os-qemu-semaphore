#include "fut.h"
#include "futasm.h"
#include "sched.h"
#include "mm.h"
#include "irq.h"

unsigned long fut_page;

#define FUT(s) *(unsigned long*)(fut_page + s)

void fut_table_init() {
    fut_page = allocate_kernel_page(); // already zeroed
    FUT(0) = 1 << 16;
    FUT(0) += 1;
}


unsigned long get_fut_page() {
    return allocate_user_page(current,fut_page);
}

unsigned long fut_new(unsigned int count) {
    fut_p(0);
    unsigned long fut = 0;
    while ((FUT(fut)) % 2) fut++;
    FUT(fut) = count << 16;
    fut_v(0);
    return fut;
}

void fut_delete(unsigned long fut) {
    FUT(fut) = 0;
}

void fut_block(unsigned long fut) {
    disable_irq();
    current->state = TASK_BLOCKED;
    current->blocked_by = fut;
    enable_irq();
    schedule();
}

void fut_p(unsigned long fut) {
    int blocked = fut_pasm(fut, fut_page);
    if (blocked) fut_block(fut);
}

void fut_v(unsigned long fut) {
    fut_vasm(fut, fut_page);
}