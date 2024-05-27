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
#include <dfs_file.h>
#include <unistd.h>

/**
 * @brief The function is used to read data from a file descriptor into the buffer.
 * 
 * @param fd the file descriptor
 * @param buf buffer
 * @param buf_size buffer size
 * @return ssize_t The number of bytes read, which returns -1 if the read operation fails.
 */
ssize_t __mlibc_sys_read(int fd, void* buf, size_t buf_size)
{
    int result;
    struct dfs_file *d;

    /* get the fd */
    d = fd_get(fd);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);

        return -1;
    }

    result = dfs_file_read(d, buf, buf_size);
    if (result < 0)
    {
        rt_set_errno(result);

        return -1;
    }

    return result;
}