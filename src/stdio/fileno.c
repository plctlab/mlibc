/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/5   0Bitbiscuits the first version
 */
#include "stdio_impl.h"
#include <errno.h>

int fileno(FILE *f)
{
    FLOCK(f);
    int fd = f->fd;
    FUNLOCK(f);
    if (fd < 0) {
        // errno = EBADF;
        return -1;
    }
    return fd;
}