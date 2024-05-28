/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6   0Bitbiscuits  realize sys_close
 */
#include <libc_config.h>
#ifdef MLIBC_RUNNING_RTTHREAD

#include <rtconfig.h>
#ifdef RT_USING_DFS

#include <sys/sys_fio.h>
#include <compiler.h>

/**
 * @brief Empty implementation.
 * 
 * @param fd The file descriptor
 * @return int
 */
mlibc_weak __mlibc_sys_close(int fd)
{
    (void)fd;

    return 0;
}

#endif /* RT_USING_DFS */
#endif /* MLIBC_RUNNING_RT_THREAD */