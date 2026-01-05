#ifndef	_SYS_H
#define	_SYS_H

#define __NR_syscalls	 9

#ifndef __ASSEMBLER__

void sys_write(char * buf);
int sys_fork();
void sys_exit();
/*-----------SEMAPHORE-----------*/
unsigned long sys_sem_new(unsigned int count);

void sys_sem_delete(unsigned long sem);
void sys_sem_p(unsigned long sem);

void sys_sem_v(unsigned long sem);


void sys_fut_get_page();
void sys_fut_block(unsigned long fut);

#endif

#endif  /*_SYS_H */
