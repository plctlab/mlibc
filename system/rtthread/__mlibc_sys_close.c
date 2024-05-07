/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6   0Bitbiscuits  realize sys_close
 */
#include <syscall.h>
#include <dfs_file.h>
#include <errno.h>
#include <unistd.h>

/**
 * @brief the function will close the open file descriptor.
 * 
 * @param fd the file descriptor
 * @return int 0 on successful, -1 on failed.
 */
int __mlibc_sys_close(int fd)
{
    return close(fd);
}