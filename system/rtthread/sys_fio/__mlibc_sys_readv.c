/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  realize sys_readv
 */
#include <sys/syscall.h>
#include <dfs_file.h>
#include <errno.h>
#include <unistd.h>

/**
 * @brief The function is used to read data from a file descriptor into multiple buffers.
 * 
 * @param fd the file descriptor.
 * @param iov buffer array
 * @param iov_size the number of buffer
 * @return ssize_t The number of bytes read, which returns -1 if the read operation fails.
 */
ssize_t __mlibc_sys_readv(int fd, iovec_t *iov, size_t iov_size)
{
    int i = 0;
    ssize_t cnt = 0;
    ssize_t ret = -EBADF;

    for(; i < iov_size; ++i)
    {
        ret = __mlibc_sys_read(fd, (iov + i)->buf, (iov + i)->buf_size);
        if(ret <= 0)
        {
            break;
        }
        cnt += ret;
    }

    if(ret < 0)
    {
        return ret;
    }
    return cnt;
}