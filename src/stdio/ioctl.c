/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <unistd.h>
#include <fcntl.h>
#include <compiler.h>
#include <errno.h>

mlibc_weak int ioctl(int fildes, int cmd, ...)
{
    (void)fildes;
    (void)cmd;

    return -EINVAL;
}