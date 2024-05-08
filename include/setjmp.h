/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits the first version
 */
#ifndef MLIBC_SETJMP_H__
#define MLIBC_SETJMP_H__

#define jmp_buf_t long *

int setjmp(jmp_buf_t env);
void longjmp(jmp_buf_t env, int val);

#endif
