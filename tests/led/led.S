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
    LI(a0, LED_ADDR)

    LI(t0, 0xffff)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x0)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x1)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x2)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x4)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x8)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x10)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x20)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x40)
    sw t0, 0(a0)

    jal wait_1s

    LI(t0, 0x80)
    sw t0, 0(a0)

    jal wait_1s
    NOP

    # b locate
    b end
    NOP

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