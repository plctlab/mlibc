/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/7    0Bitbiscuits  the first version
 */
#include <libc_config.h>
#ifdef MLIBC_RUNNING_RTTHREAD

#include <rtconfig.h>
#ifdef RT_USING_DFS

#include <sys/sys_fio.h>
#include <compiler.h>

/**
 * @brief this function is a POSIX compliant version, which will open a file and
 * return a file descriptor according specified flags.
 * 
 * @param path the path name of file.
 * @param flags the file open flags.
 * @param ... 
 * @return ssize_t the non-negative integer on successful open, others for failed.
 */
mlibc_weak int __mlibc_sys_open(const char *path, int flags, ...)
{
    (void)path;
    (void)flags;
    
    return -1;
}

#endif /* RT_USING_DFS */
#endif /* MLIBC_RUNNING_RT_THREAD */