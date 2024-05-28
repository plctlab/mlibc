/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6   0Bitbiscuits  realize sys_close
 */
#include <sys/sys_fio.h>
#include <compiler.h>

/**
 * @brief Empty implementation.
 * 
 * @param fd The file descriptor
 * @return int
 */
mlibc_weak int __mlibc_sys_close(int fd)
{
    (void)fd;

    return 0;
}
