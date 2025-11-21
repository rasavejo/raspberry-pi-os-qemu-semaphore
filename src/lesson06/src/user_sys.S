.section ".text.user"

.set SYS_WRITE_NUMBER, 0 		// syscal numbers
.set SYS_FORK_NUMBER, 1
.set SYS_EXIT_NUMBER, 2
.set SYS_SEM_NEW_NUMBER, 3
.set SYS_SEM_DELETE_NUMBER, 4
.set SYS_SEM_P_NUMBER, 5
.set SYS_SEM_V_NUMBER, 6
.set SYS_FUT_GET_PAGE_NUMBER 7


.globl user_delay
user_delay:
	subs x0, x0, #1
	bne user_delay
	ret

.globl call_sys_write
call_sys_write:
	mov w8, #SYS_WRITE_NUMBER
	svc #0
	ret

.globl call_sys_exit
call_sys_exit:
	mov w8, #SYS_EXIT_NUMBER
	svc #0
	ret

.globl call_sys_fork
call_sys_fork:
	mov w8, #SYS_FORK_NUMBER
	svc #0
	ret

.globl call_sys_sem_new
call_sys_sem_new:
	mov w8, #SYS_SEM_NEW_NUMBER
	svc #0
	ret

.globl call_sys_sem_delete
call_sys_sem_delete:
	mov w8, #SYS_SEM_DELETE_NUMBER
	svc #0
	ret

.globl call_sys_sem_P
call_sys_sem_P:
	mov w8, #SYS_SEM_P_NUMBER
	svc #0
	ret

.globl call_sys_sem_v
call_sys_sem_v:
	mov w8, #SYS_SEM_V_NUMBER
	svc #0
	ret

.globl call_sys_fut_get_page
call_sys_fut_get_page:
	mov w8, #SYS_FUT_GET_PAGE_NUMBER
	svc #0
	ret