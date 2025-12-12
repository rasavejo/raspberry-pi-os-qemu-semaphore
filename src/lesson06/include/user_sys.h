#ifndef	_USER_SYS_H
#define	_USER_SYS_H

void call_sys_write(char * buf);
int call_sys_fork();
void call_sys_exit();
unsigned long call_sys_sem_new(unsigned int count);
void call_sys_sem_delete(unsigned long sem);
void call_sys_sem_P(unsigned long sem);
void call_sys_sem_V(unsigned long sem);
unsigned long call_sys_fut_get_page();
void call_sys_fut_block();


extern void user_delay ( unsigned long);
extern unsigned long get_sp ( void );
extern unsigned long get_pc ( void );

#endif  /*_USER_SYS_H */
