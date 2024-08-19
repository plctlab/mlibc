/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/19  0Bitbiscuits the first version
 */
#include "../internal/stdio_impl.h"
#include <string.h>
#include <fcntl.h>

static int __towrite(FILE *f)
{
    if (f->flags & F_NOWR) {
        f->flags |= F_ERR;
        return EOF;
    }
    /* Clear read buffer (easier than summoning nasal demons) */

    f->rpos = f->rend = 0;

    /* Activate write through the buffer. */
    f->wpos = f->wbase = f->buf;
    f->wend = f->buf + f->buf_size;

    return 0;
}

size_t __fwrite_unlocked(const unsigned char *restrict s, size_t l, FILE *restrict f)
{
    size_t i = 0;
    size_t unused = 0;
    size_t tmp = 0;
    size_t origin_length = l;

    /* 
    ** if it was a write operation before, continue with the write operation.
    ** if not, then initialize the write buffer
    */
    if (!f->wend && __towrite(f)) return 0;

    while (l > f->wend - f->wpos)
    {
        unused = f->wend - f->wpos; // calculate the rest of write buffer
        memcpy(f->wpos, s, unused); // fill the rest of write buffer
        tmp = f->write(f, f->wbase, f->wend - f->wbase);
        /* write failure */
        if(tmp != (f->wend - f->wbase)) return origin_length - l;
        /* write success then reset write buffer */
        f->wpos = f->wbase;
        s += unused;
        l -= unused;
    }

    if (f->lbf >= 0) {
        /* Match /^(.*\n|)/ */
        for (i=l; i && s[i-1] != '\n'; i--);
        if (i) {
            size_t n = f->write(f, s, i);
            if (n < i) return n;
            s += i;
            l -= i;
        }
    }

    memcpy(f->wpos, s, l);
    f->wpos += l;
    return l + i;
}

size_t fwrite(const void *buf, size_t elem_size, size_t elem_cnt, FILE *f)
{
    size_t ret = 0;

    FLOCK(f);
    ret = __fwrite_unlocked(buf, elem_size * elem_cnt, f);
    FUNLOCK(f);

    return ret;
}