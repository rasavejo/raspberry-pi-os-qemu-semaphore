.section ".text.user"

.set SYS_WRITE_NUMBER, 0 		// syscal numbers
.set SYS_FORK_NUMBER, 1
.set SYS_EXIT_NUMBER, 2
.set SYS_SEM_NEW_NUMBER, 3
.set SYS_SEM_DELETE_NUMBER, 4
.set SYS_SEM_P_NUMBER, 5
.set SYS_SEM_V_NUMBER, 6
.set SYS_FUT_GET_PAGE_NUMBER, 7
.set SYS_FUT_BLOCK_NUMBER, 8


.globl user_delay
user_delay:
	subs x0, x0, #1
	bne user_delay
	ret


.globl write_memory
write_memory:
	str x0, [x1]
	ret

.globl read_memory
read_memory:
	ldr x0, [x0]
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

.globl call_sys_sem_V
call_sys_sem_V:
	mov w8, #SYS_SEM_V_NUMBER
	svc #0
	ret

.globl call_sys_fut_get_page
call_sys_fut_get_page:
	mov w8, #SYS_FUT_GET_PAGE_NUMBER
	svc #0
	ret

.globl call_sys_fut_block
call_sys_fut_block:
	mov w8, #SYS_FUT_BLOCK_NUMBER
	svc #0
	ret




.globl fut_pasm
fut_pasm :
    add x1,x0,x1
fut_p_try :

    LDXR x0,[x1]
    CMP x0,#0
    BNE fut_p_true
    MOV x0, #1
    RET
fut_p_true : 
    SUB x0, x0, #1
    STXR w0,x2,[x1]
    CMP x0,#0
    BNE fut_p_try
    
    ret


.globl fut_vasm
fut_vasm :
    add x1,x0,x1
fut_v_try :

    LDXR x0,[x1]
    ADD x0, x0, #1
    STXR w0,x2,[x1]
    CMP x0,#0
    BNE fut_v_try
    
    ret

