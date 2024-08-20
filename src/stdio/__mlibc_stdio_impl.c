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
    ssize_t n = 0;
    ssize_t dirty_file_buffer_len = 0;

    if (f->flags & F_APP)
    {
        lseek(f->fd, (off_t) 0, SEEK_END);
    }
    
    /* Write file buffer */
    n = f->wpos - f->wbase;
    dirty_file_buffer_len = n;
    while(n > 0)
    {
        t = write(f->fd, f->wbase + dirty_file_buffer_len - n, n);
        if(t < 0)
        {
            f->flags = F_ERR;
            // write error then clean the write buffer
            f->wpos = f->wbase = f->wend = 0;
            return dirty_file_buffer_len - n;
        }
        n -= t;
    }
    f->wpos = 0; // reset write pointer

    /* Write user buffer */
    n = buf_size;
    while(n > 0)
    {
        t = write(f->fd, buf + buf_size - n, n);
        if(t < 0)
        {
            f->flags = F_ERR;
            return buf_size - n + dirty_file_buffer_len;
        }
        n -= t;
    }

    return buf_size + dirty_file_buffer_len;
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
