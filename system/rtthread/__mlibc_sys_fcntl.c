/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  realize sys_fcntl
 */
#include "../internal/syscall.h"
#include <stdarg.h>
#include <dfs_file.h>

/**
 * @brief this function is a POSIX compliant version, which shall perform a variety of
 * control functions on devices.
 * 
 * @param fd file descriptor
 * @param cmd file operation command
 * @param ... operation args
 * @return int 0 on successful completion. Otherwise, -1 shall be returned and errno
 * set to indicate the error.
 */
int __mlibc_sys_fcntl(int fd, int cmd, ...)
{
    int res = 0;
    va_list args;

    va_start(args, cmd);
    res = fcntl(fd, cmd, args);
    va_end(args);

    return res;
}
