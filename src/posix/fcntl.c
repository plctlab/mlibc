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

mlibc_weak int fcntl(int fd, int cmd, ... /* arg */ )
{
    (void)fd;
    (void)cmd;
    
    return -1;
}