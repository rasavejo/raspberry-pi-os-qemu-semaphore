#ifndef _SEM_H
#define _SEM_H



typedef unsigned int semaphore; // xxxxxxxxxxxxxxxxyyyyyyyyyyyyyyyy with x the semaphore count and y a bitfield, the last one is whether the semaphore is currently used

void sem_table_init();
unsigned long sem_new(unsigned int count);

void sem_delete(semaphore sem);
void sem_p(semaphore sem);

void sem_v(semaphore sem);




#endif