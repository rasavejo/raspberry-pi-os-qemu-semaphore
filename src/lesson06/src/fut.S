.globl fut_pasm
fut_pasm :
    mov x16, =fut_page
    add x16,x0,x16


fut_p_try :

    LDXR x0,[x16]
    CMP x0,#0
    MOVEQ x0, #1
    RETEQ
    SUB x0, x0, #1
    STXR w0,x1,[x16]
    CMP x0,#0
    BNE fut_p_try
    
    ret


.globl fut_vasm
fut_vasm :
    mov x16, =fut_page
    add x16,x0,x16
fut_v_try :

    LDXR x0,[x16]
    ADD x0, x0, #1
    STXR w0,x1,[x16]
    CMP x0,#0
    BNE fut_v_try
    
    ret

