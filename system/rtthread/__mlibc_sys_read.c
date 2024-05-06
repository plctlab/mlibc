/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  realize sys_read
 */
#include "../internal/syscall.h"
#include <dfs_file.h>

/**
 * @brief The function is used to read data from a file descriptor into the buffer.
 * 
 * @param fd the file descriptor
 * @param buf buffer
 * @param buf_size buffer size
 * @return ssize_t The number of bytes read, which returns -1 if the read operation fails.
 */
ssize_t __mlibc_sys_read(int fd, unsigned char *buf, size_t buf_size)
{
    return read(fd, buf, buf_size);
}
