/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/29  0Bitbiscuits the first
 */
#include "../crt0.h"
#include <compiler.h>

static void
mlibc_used
mlibc_section(".init")
_cstart(void)
{ 
    __start();
}

void
mlibc_used
mlibc_section(".text.init.enter")
_start(void)
{
    /* Initialize stack */
    __asm__("adrp x1, __stack");
    __asm__("add  x1, x1, :lo12:__stack");
    __asm__("mov sp, x1");
    /* Enable FPU */
    __asm__("mov x1, #(0x3 << 20)");
    __asm__("msr cpacr_el1,x1");
    /* Jump into C code */
    __asm__("bl _cstart");
}