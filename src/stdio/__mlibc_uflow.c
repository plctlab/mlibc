/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/20  0Bitbiscuits the first version
 */
#include "../internal/stdio_impl.h"

int __mlibc_uflow(FILE *f)
{
    unsigned char c;
    if (!__mlibc_toread(f) && f->read(f, &c, 1) == 1) return c;
    return EOF;
}