#include "cpu.h"

.set	noreorder
.globl	start
start:
    LI(t0, 0x0004)
    LI(t1, 0x0002)
    addu t2, t0, t1
    b locate
    NOP

locate:
	.set noreorder
    LI(a0, SMG_ADDR)

    // 0000
    LI(t0, 0x1fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x2fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x4fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x8fc)
    sw t0, 0(a0)
    NOP4

    jal wait_1s
    NOP4

    // 0001
    LI(t0, 0x160)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x2fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x4fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x8fc)
    sw t0, 0(a0)
    NOP4

    jal wait_1s
    NOP4

    // 0002
    LI(t0, 0x1da)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x2fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x4fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x8fc)
    sw t0, 0(a0)
    NOP4

    jal wait_1s
    NOP4

    // 0003
    LI(t0, 0x1f2)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x2fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x4fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x8fc)
    sw t0, 0(a0)
    NOP4

    jal wait_1s
    NOP4

    // 0004
    LI(t0, 0x166)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x2fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x4fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x8fc)
    sw t0, 0(a0)
    NOP4

    jal wait_1s
    NOP4

    // 0005
    LI(t0, 0x1b6)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x2fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x4fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x8fc)
    sw t0, 0(a0)
    NOP4

    jal wait_1s
    NOP4

    // 0006
    LI(t0, 0x1be)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x2fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x4fc)
    sw t0, 0(a0)
    NOP4

    LI(t0, 0x8fc)
    sw t0, 0(a0)
    NOP4

    jal wait_1s
    NOP4

    # b locate
    b end
    NOP4

wait_1s:                                                                              
    LI(t2, CYCLE_NUMS_1S);                                                                         
sub1:                                                                                          
    subu t2, t2, 1;                                                                                
    bne t2, zero, sub1;                                                                            
    NOP
    jr ra
    NOP

end:
    NOP4