#ifndef	_SYS_H
#define	_SYS_H

#define __NR_syscalls	 9

#ifndef __ASSEMBLER__

void sys_write(char * buf);
int sys_fork();


void sys_fut_get_page();
void sys_fut_block(unsigned long fut);

#endif

#endif  /*_SYS_H */
