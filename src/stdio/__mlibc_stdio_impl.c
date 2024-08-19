/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7   0Bitbiscuits  the first version
 */
#include "../internal/stdio_impl.h"
#include <unistd.h>
#include <sys/errno.h>
#include <fcntl.h>

ssize_t __mlibc_read(FILE *f, unsigned char *buf, size_t buf_size)
{
    register ssize_t ret;

    ret = read(f->fd, buf, buf_size);
    if (ret >= 0)
    {
        f->off += ret;
    }
    
    return ret;
}

ssize_t __mlibc_write(FILE *f, const unsigned char *buf, size_t buf_size)
{
    ssize_t t = 0;
    ssize_t n = buf_size;

    if (f->flags & F_APP)
    {
        lseek(f->fd, (off_t) 0, SEEK_END);
    }
    
    while(n > 0)
    {
        t = write(f->fd, buf, n);
        if(t < 0)
        {
            f->flags = F_ERR;
            // write error then clean the write buffer
            f->wpos = f->wbase = f->wend = 0;
            return buf_size - n;
        }
        n -= t;
    }

    return buf_size;
}

off_t __mlibc_lseek(FILE *f, off_t offset, int whence)
{
    register off_t ret = 0;

    ret = lseek(f->fd, offset, whence);
    if(ret >= 0)
    {
        f->off = ret;
    }

    return ret;
}

int __mlibc_close(FILE *f)
{
    return close(f->fd);
}
