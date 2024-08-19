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

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int __toread(FILE* f)
{
    // write buffer is dirty then write it into file
    if (f->wpos != f->wbase) f->write(f, f->wpos, f->wpos - f->wbase);
    // clean write buffer
    f->wpos = f->wbase = f->wend = 0;
    // check file flags
    if (f->flags & F_NORD) {
        f->flags |= F_ERR;
        return EOF;
    }
    // set read buffer pointer
    f->rpos = f->rend = f->buf + f->buf_size;
    return (f->flags & F_EOF) ? EOF : 0;
}

size_t fread(void *buf, size_t elem_size, size_t elem_cnt, FILE *f)
{
    unsigned char *dest = buf;
    size_t len = elem_size * elem_cnt;
    size_t l = len;
    size_t k = 0;
    if (!elem_size) elem_cnt = 0;

    FLOCK(f);

    if (f->rpos != f->rend) {
        /* First exhaust the buffer. */
        k = MIN(f->rend - f->rpos, l);
        memcpy(dest, f->rpos, k);
        f->rpos += k;
        dest += k;
        l -= k;
    }
    
    /* Read the remainder directly */
    for (; l; l-=k, dest+=k) {
        k = __toread(f) ? 0 : f->read(f, dest, l);
        if (!k) {
            FUNLOCK(f);
            return (len - l) / elem_size;
        }
    }

    FUNLOCK(f);
    return elem_cnt;
}