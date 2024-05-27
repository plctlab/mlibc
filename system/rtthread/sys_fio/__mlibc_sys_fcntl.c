/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  realize sys_fcntl
 */
#include <sys/sys_fio.h>
#include <stdarg.h>
#include <dfs_file.h>
#include <unistd.h>

/**
 * @brief this function is a POSIX compliant version, which shall perform a variety of
 * control functions on devices.
 * 
 * @param fd file descriptor
 * @param cmd file operation command
 * @param ... operation args
 * @return int 0 on successful completion. Otherwise, -1 shall be returned and errno
 * set to indicate the error.
 */
int __mlibc_sys_fcntl(int fd, int cmd, ...)
{
    int ret = -1;
    struct dfs_file *d;

    /* get the fd */
    d = fd_get(fd);
    if (d)
    {
        void *arg;
        va_list ap;

        va_start(ap, cmd);
        arg = va_arg(ap, void *);
        va_end(ap);

        ret = dfs_file_ioctl(d, cmd, arg);
    }
    else ret = -EBADF;

    if (ret < 0)
    {
        rt_set_errno(ret);
        ret = -1;
    }

    return ret;
}
