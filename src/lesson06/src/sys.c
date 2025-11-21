#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "sched.h"
#include "mm.h"
#include "sem.h"


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

void sys_sem_delete(unsigned long sem){
	printf("Delete sem\n");
	sem_delete(sem);

}
void sys_sem_p(unsigned long sem){
	printf("Take sem\n");
	sem_p(sem);

}

void sys_sem_v(unsigned long sem){
	printf("Drop sem\n");
	sem_v(sem);
}


void * const sys_call_table[] = {sys_write, sys_fork, sys_exit,sys_sem_new,sys_sem_delete,sys_sem_p,sys_sem_v};
