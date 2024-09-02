/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/23  0Bitbiscuits the first version
 */
#include "../crt0.h"
#include <compiler.h>

extern char __stack[];

static void
mlibc_used
mlibc_section(".init")
_cstart(void)
{
    __start();
}

void 
mlibc_neck 
mlibc_used 
mlibc_section(".init") 
_start(void)
{
    /* Generate a reference to __vector_table so we get one loaded */
    __asm__(".equ __my_vector_table, __vector_table");

#ifdef __thumb2__
    /* Make exceptions run in Thumb mode */
    uint32_t sctlr;
    __asm__("mrc p15, 0, %0, c1, c0, 0" : "=r" (sctlr));
    sctlr |= (1 << 30);
    __asm__("mcr p15, 0, %0, c1, c0, 0" : : "r" (sctlr));
#endif
#if defined __ARM_FP || defined __ARM_FEATURE_MVE
#if __ARM_ARCH > 6
    /* Set CPACR for access to CP10 and 11 */
    __asm__("mcr p15, 0, %0, c1, c0, 2" : : "r" (0xf << 20));
#endif
    /* Enable FPU */
    __asm__("vmsr fpexc, %0" : : "r" (0x40000000));
#endif

    /* Initialize stack pointer */
    __asm__("mov sp, %0" : : "r" (__stack));
    /* Branch to C code */
    __asm__("b _cstart");
}