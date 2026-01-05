#include "user_fut.h"
#include "user_sys.h"
//#include "futasm.h"

unsigned long fut_new(unsigned long page, unsigned int count) {
    fut_p(page, 0);
    unsigned long fut = 0;
    while (((FUT(page, fut)) & 1) == 1)
        fut++;
    FUT(page, fut) = count << 16 | 1;
    fut_v(page, 0);
    return fut;
}

void fut_delete(unsigned long page, unsigned long fut) {
    FUT(page, fut) = 0;
    return;
}

void fut_p(unsigned long page, unsigned long fut) {
    int count = fut_count(page, fut);
    if (count == 1)
        call_sys_write("FUT = 1\n");
    else
        call_sys_write("FUT = 0\n");

    int blocked = fut_pasm(fut, page);
    if (blocked == 1) {
        call_sys_write("BLOCKED\n");
        call_sys_fut_block(fut);
    }
    count = fut_count(page, fut);
    if (count == 1)
        call_sys_write("FUT = 1\n");
    else
        call_sys_write("FUT = 0\n");

    return;
}

void fut_v(unsigned long page, unsigned long fut) {
    fut_vasm(fut, page);
    if (fut_count(page, fut) == 1)
        call_sys_write("FUT v = 1\n");
    else
        call_sys_write("FUT v = 0\n");
    return;
}

int fut_count(unsigned long page, unsigned long fut) { return FUT(page, fut) >> 16; }