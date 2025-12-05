#ifndef _FUT_H
#define _FUT_H





unsigned long get_fut_page();
void fut_table_init();
unsigned long sem_new(unsigned int count);

void fut_delete(unsigned long fut);
void fut_p(unsigned long fut);

void fut_v(unsigned long fut);

void fut_block(unsigned long fut);




#endif