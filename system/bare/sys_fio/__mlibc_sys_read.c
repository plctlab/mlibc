/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  realize sys_read
 */
#include <sys/sys_fio.h>
#include <compiler.h>

/**
 * @brief The function is used to read data from a file descriptor into the buffer.
 * 
 * @param fd the file descriptor
 * @param buf buffer
 * @param buf_size buffer size
 * @return ssize_t The number of bytes read, which returns -1 if the read operation fails.
 */
mlibc_weak ssize_t __mlibc_sys_read(int fd, void* buf, size_t buf_size)
{
    (void)fd;
    (void)buf;
    (void)buf_size;

    return -1;
}