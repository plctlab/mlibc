/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/29  0Bitbiscuits the first version
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
mlibc_neck
mlibc_used
mlibc_section(".text.startup")
_start(void)
{
    /* Init sp and gp */
    __asm__(".option    push\n"
            ".option    norelax\n"
            "la         sp, __stack\n"
            "la         gp, __global_pointer$\n"
            ".option    pop");
    /* Enable FPU(todo) */

    /* Jump to _cstart */
    __asm__("j         _cstart");
}