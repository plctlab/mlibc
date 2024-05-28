/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  the first version
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
mlibc_weak off_t __mlibc_sys_lseek(int fd, off_t offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;
    
    return -1;
}