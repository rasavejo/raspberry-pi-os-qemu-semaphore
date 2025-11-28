#ifndef	_SYS_H
#define	_SYS_H

#define __NR_syscalls	 8

#ifndef __ASSEMBLER__

void sys_write(char * buf);
int sys_fork();
void sys_exit();
/*-----------SEMAPHORE-----------*/
unsigned long sys_sem_new(unsigned int count);

void sys_sem_delete(unsigned long sem);
void sys_sem_p(unsigned long sem);

void sys_sem_v(unsigned long sem);

#endif

#endif  /*_SYS_H */
