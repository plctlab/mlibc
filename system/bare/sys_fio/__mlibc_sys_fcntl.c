/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  realize sys_fcntl
 */
#include <libc_config.h>
#ifdef MLIBC_RUNNING_RTTHREAD

#include <rtconfig.h>
#ifdef RT_USING_DFS

#include <sys/sys_fio.h>
#include <compiler.h>

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
mlibc_weak int __mlibc_sys_fcntl(int fd, int cmd, ...)
{
    (void)fd;
    (void)cmd;
    
    return 0;
}

#endif /* RT_USING_DFS */
#endif /* MLIBC_RUNNING_RT_THREAD */