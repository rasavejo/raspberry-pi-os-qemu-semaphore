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

