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
    /* write buffer is uninitialized */
    if (!f->wend && __mlibc_towrite(f)) return EOF;
    /* write char to buffer */
    if (f->wpos != f->wend && c != f->lbf) return *f->wpos++ = c;
    /* write '\n' then flush buffer */
    if (f->write(f, &c, 1) == EOF) return EOF;
    return c;
}
