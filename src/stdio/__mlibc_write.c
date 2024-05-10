/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7   0Bitbiscuits  the first version
 */
#include <sys/syscall.h>
#include <stdio_impl.h>
#include <fcntl.h>

size_t __mlibc_write(FILE *f, unsigned char *buf, size_t buf_size)
{
    iovec_t iov[2] = {
        { .buf = f->wbase, .buf_size = f->wpos - f->wbase },
        { .buf = buf, .buf_size = buf_size}
    };
    iovec_t *iov_p;
    size_t total = iov_p[0].buf_size + iov_p[1].buf_size;
    ssize_t ret = 0;
    int iov_cnt = 2;

    while(1)
    {
        ret = __mlibc_sys_writev(f->fd, iov_p, iov_cnt);
        if(ret == total) // all data write into file
        {
            /* reset the write pointer of the buffer. */
            f->wend = f->buf + f->buf_size;
            f->wpos = f->wbase = f->buf;
            return buf_size;
        }
        if(ret < 0) // write failed
        {
            f->wpos = f->wbase = f->wend = 0;
            f->flags |= F_ERR;
            return iov_cnt == 2 ? 0 : buf_size - iov_p[0].buf_size;
        }
        /* wrote partial data. */
        total -= ret;
        if(ret > iov_p[0].buf_size)
        {
            /* wrote all file buffer data and partial user data */
            ret -= iov_p[0].buf_size;
            iov_p++;
            iov_cnt--;
        }
        iov_p[0].buf = (char *)iov_p[0].buf + ret;
        iov_p[0].buf_size -= ret;
    }

}