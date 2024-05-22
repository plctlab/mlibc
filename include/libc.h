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

int _libc_init(void);

#endif /* MLIBC_LIBC_H__ */