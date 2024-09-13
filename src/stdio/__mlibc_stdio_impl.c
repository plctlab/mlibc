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
    ssize_t user_buffer_cnt = 0;
    ssize_t file_buffer_cnt = 0;

    /* Read to the user buffer */
    user_buffer_cnt = read(f->fd, buf, buf_size);
    if(user_buffer_cnt <= 0)
    {
        f->flags |= user_buffer_cnt ? F_ERR : F_EOF;
		return 0;
    }
    else if(user_buffer_cnt < buf_size)
    {
        return user_buffer_cnt;
    }

    /* Read to the file buffer */
    file_buffer_cnt = f->buf_size > 0 ? read(f->fd, f->buf, f->buf_size) : 0;
    if(file_buffer_cnt <= 0)
    {
        f->flags |= file_buffer_cnt ? F_ERR : F_EOF;
		return user_buffer_cnt;
    }
    f->rpos = f->buf;
	f->rend = f->buf + file_buffer_cnt;

    return user_buffer_cnt;
}

/* 
** return the number has been written
** if an error occurs, return EOF
*/
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
            return EOF;
        }
        n -= t;
    }
    /* reset write buffer */
    f->wpos = f->wbase = f->buf; 
    f->wend = f->buf + f->buf_size;

    /* Write user buffer */
    n = buf_size;
    while(n > 0)
    {
        t = write(f->fd, buf + buf_size - n, n);
        if(t < 0)
        {
            f->flags = F_ERR;
            return EOF;
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
