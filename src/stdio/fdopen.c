/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/23   0Bitbiscuits  the first version
 */
#include "../internal/stdio_impl.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <compiler.h>
#include <errno.h>

/**
 * @brief This function is not responsible for creating files,
 * it primarily handles associating the fd with the FILE structure.
 * 
 * @param fd The file descriptor
 * @param mode The file open mode.
 * support:
 * r: you can readonly
 * w: clean the file and writeonly
 * a: append the file
 * r+: you can read and write file
 * w+: file will clean first and you can read and write file
 * a+: append the file
 * @return FILE* A pointer to the FILE structure associated with the file descriptor.
 */
mlibc_weak FILE *fdopen(int fd, const char *mode)
{
    FILE *f;

    /* check mode */
    if(!strchr("rwa", *mode)) 
    {
        /* error operation */
        errno = EINVAL;
        return NULL;
    }

    /* allocate memory for file and buffer */
    if(!(f = malloc(sizeof(FILE) + UNGET + BUFSIZ)))
    {
        return NULL;
    }

    /* zero-fill FILE */
    memset(f, 0, sizeof *f);

    if(!strchr(mode, '+'))
    {
        f->flags = (*mode == 'r') ? F_NOWR : F_NORD;
    }

    /* Set append mode on fd if opened for append */
    if (*mode == 'a') {
        int flags = fcntl(fd, F_GETFL);

        if (!(flags & O_APPEND))
        { 
            fcntl(fd, F_SETFL, flags | O_APPEND);
        }
        f->flags |= F_APP;
    }

    f->fd = fd;
    f->buf = (unsigned char *)f + sizeof(FILE) + UNGET;
    f->buf_size = BUFSIZ;

    f->lbf = EOF;
    if (!(f->flags & F_NOWR))
        f->lbf = '\n';
    
    FLOCK_INIT(f);

    /* Initialize op ptrs. No problem if some are unneeded. */
    f->read = __mlibc_read;
    f->write = __mlibc_write;
    f->seek = __mlibc_lseek;
    f->close = __mlibc_close;

    return __mlibc_file_add(f);
}