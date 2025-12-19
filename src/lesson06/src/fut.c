#include "fut.h"
#include "sched.h"
#include "mm.h"
#include "irq.h"
#include "printf.h"

unsigned long fut_page;

#define FUT(page,s) *(unsigned long*)(page + s*32)


void fut_table_init() {
    fut_page = allocate_kernel_page(); // already zeroed
    FUT(fut_page, 0) = (1 << 16) | 1;
}

unsigned long get_fut_page(){
    // for(int i = 0; i < current->mm.user_pages_count; i++){
    //     printf("%d physic %d virt %d\n" ,i, (int)current ->mm.user_pages[i].phys_addr,  (int)current ->mm.user_pages[i].virt_addr);
    // }
    unsigned long virt_addr = current->mm.user_pages[current->mm.user_pages_count-1].virt_addr + PAGE_SIZE;
    map_page(current, virt_addr, fut_page - VA_START);
    // for(int i = 0; i < current->mm.user_pages_count; i++){
    //     printf("%d physic %d virt %d\n" ,i, (int)current ->mm.user_pages[i].phys_addr,  (int)current ->mm.user_pages[i].virt_addr);
    // }
	return virt_addr;
}

void fut_block(unsigned long fut) {
    disable_irq();
    current->state = TASK_BLOCKED;
    current->blocked_by = fut;
    enable_irq();
    schedule();
}

