/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/28   0Bitbiscuits the first version
 */

#include <fcntl.h>
#include <compiler.h>

mlibc_weak int open(const char *file, int flags, ...)
{
    (void)file;
    (void)flags;

    return -1;
}