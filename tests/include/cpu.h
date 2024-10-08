#ifndef CPU
#define CPU

#include "regdef.h"

#define TIME_FREQ 5
#define CYCLE_NUMS_1S (TIME_FREQ * 1)

// #define TIME_FREQ 100
// #define CYCLE_NUMS_1S (TIME_FREQ * 1000 * 1000)

#define LED_ADDR 0xffff0000
#define SMG_ADDR 0xffff0001

#define NOP addu zero, zero, zero
#define NOP4                                                                                       \
    NOP;                                                                                           \
    NOP;                                                                                           \
    NOP;                                                                                           \
    NOP

#define LI(reg, imm)                                                                               \
    lui reg, (imm >> 16) & 0x0000ffff;                                                             \
    addiu reg, reg, (imm & 0x0000ffff)

#endif