/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7   0Bitbiscuits  the first version
 */
#ifndef MLIBC_LIBC_H__
#define MLIBC_LIBC_H__

typedef struct
{
    unsigned char RUN_IN_OS;
} MLIBC;

extern MLIBC mlibc;

void __mlibc_heap_sys_init(void);
void __mlibc_heap_nosys_init(void *begin_addr, void *end_addr);

#endif /* MLIBC_LIBC_H__ */