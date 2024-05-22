/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6   0Bitbiscuits  realize sys_close
 */
#include <porting/porting_fio.h>
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
    int result;
    struct dfs_file *d;

    d = fd_get(fd);
    if (d == NULL)
    {
        rt_set_errno(-EBADF);
        return -1;
    }

    result = dfs_file_close(d);
    fd_release(fd);

    if (result < 0)
    {
        rt_set_errno(result);

        return -1;
    }

    return 0;
}