#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "sched.h"
#include "mm.h"
#include "sem.h"
#include "fut.h"

void sys_write(char * buf){
	printf(buf);
}

int sys_fork(){
	return copy_process(0, 0, 0);
}

void sys_exit(){
	exit_process();
}
/*-----------SEMAPHORE-----------*/
unsigned long sys_sem_new(unsigned int count){
	printf("Acquire new sem\n");
	return sem_new(count);
}

void sys_sem_delete(semaphore sem){
	printf("Delete sem\n");
	sem_delete(sem);

}
void sys_sem_p(semaphore sem){
	printf("Take sem\n");
	sem_p(sem);

}

void sys_sem_v(semaphore sem){
	printf("Drop sem\n");
	sem_v(sem);
}

void sys_fut_get_page() {
	printf("Get futex table\n");
	get_fut_page();
}

void sys_fut_block(unsigned long fut) {
	printf("Blocking task %s\n",fut);
	fut_block(fut);
}


void * const sys_call_table[] = {sys_write, sys_fork, sys_exit,sys_sem_new,sys_sem_delete,sys_sem_p,sys_sem_v,sys_fut_get_page,sys_fut_block};
