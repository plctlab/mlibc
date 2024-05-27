/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  realize sys_ioctl
 */
#include <sys/sys_fio.h>
#include <stdarg.h>
#include <dfs_file.h>
#include <unistd.h>

/**
 * @brief  * this function is a POSIX compliant version, which shall perform a variety of
 * control functions on devices.
 * 
 * @param fildes the file description
 * @param cmd the specified command
 * @param ... represents the additional information that is needed by this
 * specific device to perform the requested function.
 *
 * @return 0 on successful completion. Otherwise, -1 shall be returned and errno
 * set to indicate the error.
 */
int __mlibc_sys_ioctl(int fd, int cmd, ...)
{
    void *arg;
    va_list ap;

    va_start(ap, cmd);
    arg = va_arg(ap, void *);
    va_end(ap);

    /* we use fcntl for this API. */
    return __mlibc_sys_fcntl(fd, cmd, arg);
}
