/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/29   0Bitbiscuits the first version
 */
#include "../internal/stdio_impl.h"
#include <compiler.h>
#include <fcntl.h>
#include <errno.h>

int ferror(FILE *f)
{
    int ret = 0;
    
    FLOCK(f);
    ret = !!(f->flags & F_ERR);
    FUNLOCK(f);

    return ret;
}