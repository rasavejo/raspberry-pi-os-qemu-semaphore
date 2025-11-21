#ifndef _SEM_H
#define _SEM_H



unsigned long fut_page;


unsigned long get_fut_page();
void fut_table_init();
unsigned long sem_new(unsigned int count);

void fut_delete(unsigned long sem);
void fut_p(unsigned long sem);

void fut_v(unsigned long fut);




#endif