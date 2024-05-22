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
 * @brief The function is used to read data from a file descriptor into the buffer.
 * 
 * @param fd the file descriptor
 * @param buf buffer
 * @param buf_size buffer size
 * @return ssize_t The number of bytes read, which returns -1 if the read operation fails.
 */
off_t __mlibc_sys_lseek(int fd, off_t offset, int whence)
{
    int result;
    struct dfs_file *d;

    d = fd_get(fd);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);

        return -1;
    }

    switch (whence)
    {
    case SEEK_SET:
        break;

    case SEEK_CUR:
        offset += d->pos;
        break;

    case SEEK_END:
        offset += d->vnode->size;
        break;

    default:
        rt_set_errno(-EINVAL);

        return -1;
    }

    if (offset < 0)
    {
        rt_set_errno(-EINVAL);

        return -1;
    }
    result = dfs_file_lseek(d, offset);
    if (result < 0)
    {
        rt_set_errno(result);

        return -1;
    }

    return offset;
}
