/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7    0Bitbiscuits  the first version
 */
#include <sys/syscall.h>
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
    int fd, result;
    struct dfs_file *d;

    /* allocate a fd */
    fd = fd_new();
    if (fd < 0)
    {
        rt_set_errno(-ENOMEM);

        return -1;
    }
    d = fd_get(fd);

    result = dfs_file_open(d, path, flags);
    if (result < 0)
    {
        /* release the ref-count of fd */
        fd_release(fd);

        rt_set_errno(result);

        return -1;
    }

    return fd;
}
