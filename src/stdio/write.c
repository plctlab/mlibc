/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/28   0Bitbiscuits the first version
 */

#include <unistd.h>
#include <compiler.h>

mlibc_weak ssize_t write(int fd, const void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;

    return -1;
}