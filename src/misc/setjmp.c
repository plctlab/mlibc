/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  undefine
 */
#include <setjmp.h>


int setjmp(jmp_buf_t env)
{
    return 0;
}

void longjmp(jmp_buf_t env, int val)
{
    return;
}


