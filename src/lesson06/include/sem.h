#ifndef _SEM_H
#define _SEM_H



typedef unsigned int semaphore; // xxxxxxxxxxxxxxxxyyyyyyyyyyyyyyyy with x the semaphore count and y a bitfield, the last one is whether the semaphore is currently used

void sem_table_init();
unsigned long sem_new(unsigned int count);

void sem_delete(unsigned long sem);
void sem_p(unsigned long sem);

void sem_v(unsigned long sem);
unsigned int sem_count(unsigned long sem);



#endif