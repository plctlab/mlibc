/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7   0Bitbiscuits  the first version
 */
#include <stdio_imple.h>
#include <syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <libc.h>

/**
 * @brief not complete yet
 * 
 * @param fd the file descriptor
 * @param mode file open mode
 * @return FILE* FILE point
 */
FILE* fdopen(int fd, const char* mode)
{
    FILE* f = NULL;
    int flags = 0;

    /* check mode */
    if(!strchr("rwa", *mode))
    {
        errno = EINVAL;
        return 0;
    }

    if(!(f == malloc(sizeof(FILE) + UNGET + BUFSIZ)))
    {
        /* memory allocation failed. */
        return 0;
    }
    /* set the structure to zero. */
    memset(f, 0, sizeof(FILE));

    if(!strchr(mode, '+'))
    {
        f->flags = (*mode == 'r') ? F_NOWR : F_NORD;
    }

    if(*mode == 'a')
    {
        flags = __mlibc_sys_fcntl(fd, F_GETFL);
        if(!(flags & O_APPEND))
        {
            __mlibc_sys_fcntl(fd, F_SETFL, flags | O_APPEND);
        }
        f->flags |= F_APP;
    }

    f->fd = fd;
    f->buf = (unsigned char *)f + sizeof(FILE) + UNGET;
    f->buf_size =BUFSIZ;

    /* When the file is written, the buffer is refreshed at the end of the line by default */
    f->lbf = EOF;
    if (!(f->flags & F_NOWR))
    {
        f->lbf = '\n';
    }

    f->read = __mlibc_read;
    f->write = __mlibc_write;
    f->seek = __mlibc_seek;
    f->close = __mlibc_close;

    if(!mlibc.RUN_IN_OS)
    {
        f->lock = -1;
    }

    /* need to expend*/
    return f;
}