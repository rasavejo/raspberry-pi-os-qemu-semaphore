#include "user_fut.h"
#include "user_sys.h"
//#include "futasm.h"


unsigned long fut_new(unsigned long page, unsigned int count){
    fut_p(page,0); 
    unsigned long fut = 0;
    while ((FUT(page, fut)) % 2) fut++;
    FUT(page,fut) = count << 16;
    fut_v(page, 0);
    return fut;
}


void fut_delete(unsigned long page, unsigned long fut){
  FUT(page,fut) = 0;
  return;
}

void fut_p(unsigned long page, unsigned long fut) {
 int blocked = fut_pasm(fut, page);
 if (blocked) call_sys_fut_block(fut);
 return;
}

void fut_v(unsigned long page, unsigned long fut) {
  fut_vasm(fut, page);
  return;
}
