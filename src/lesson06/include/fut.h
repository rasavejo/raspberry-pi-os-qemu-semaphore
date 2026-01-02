#ifndef _FUT_H
#define _FUT_H

unsigned long get_kernel_fut_page();

unsigned long get_fut_page();
void fut_table_init();
unsigned long fut_new(unsigned long page,unsigned int count);

void fut_delete(unsigned long page,unsigned long fut);
void fut_p(unsigned long page,unsigned long fut);

void fut_v(unsigned long page,unsigned long fut);

void fut_block(unsigned long fut);

#endif  /* _FUT_H */