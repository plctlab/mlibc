/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  the first version
 */
#include <sys/syscall.h>
#include <dfs_file.h>
#include <unistd.h>

/**
 * @brief The function is used to write data to buffer
 * 
 * @param fd the file descriptor
 * @param buf buffer
 * @param buf_size buffer size
 * @return ssize_t The number of bytes we write, which returns -1 if the write operation fails.
 */
ssize_t __mlibc_sys_write(int fd, unsigned char *buf, size_t buf_size)
{
    return write(fd, buf, buf_size);
}
