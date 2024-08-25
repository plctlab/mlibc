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

int __mlibc_overflow(FILE *f, int _c)
{
    unsigned char c = _c;
    if (!f->wend && __mlibc_towrite(f)) return EOF;
    if (f->wpos != f->wend && c != f->lbf) return *f->wpos++ = c;
    if (f->write(f, &c, 1) != 1) return EOF;
    return c;
}
