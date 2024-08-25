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
#include <fcntl.h>

int ungetc(int c, FILE *f)
{
    if (c == EOF) return c;

    FLOCK(f);

    if (!f->rpos) __mlibc_toread(f);
    if (!f->rpos || f->rpos <= f->buf - UNGET) {
        FUNLOCK(f);
        return EOF;
    }

    *--f->rpos = c;
    f->flags &= ~F_EOF;

    FUNLOCK(f);
    return (unsigned char)c;
}