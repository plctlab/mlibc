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

mlibc_weak int fcntl(int fd, int cmd, ... /* arg */ )
{
    (void)fd;
    (void)cmd;

    return -EINVAL;
}