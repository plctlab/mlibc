/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7    0Bitbiscuits  the first version
 */
#include <syscall.h>
#include <dfs_file.h>
#include <stdarg.h>

/**
 * @brief this function is a POSIX compliant version, which will open a file and
 * return a file descriptor according specified flags.
 * 
 * @param path the path name of file.
 * @param flags the file open flags.
 * @param ... 
 * @return ssize_t the non-negative integer on successful open, others for failed.
 */
int __mlibc_sys_open(const char *path, int flags, ...)
{
    int ret = 0;
    va_list args;

    va_start(args, flags);
    ret = open(path, flags, args);
    va_end(args);

    return ret;
}
