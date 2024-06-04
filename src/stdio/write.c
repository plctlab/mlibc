/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits  the first version
 */
#include <unistd.h>
#include <compiler.h>
#include <errno.h>

mlibc_weak ssize_t write(int fd, const void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;

    return -1;
}