/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  the first version
 */
#include <libc_config.h>
#ifdef MLIBC_RUNNING_RTTHREAD

#include <rtconfig.h>
#ifdef RT_USING_DFS

#include <sys/sys_fio.h>
#include <compiler.h>

/**
 * @brief The function is used to write data to buffer
 * 
 * @param fd the file descriptor
 * @param buf buffer
 * @param buf_size buffer size
 * @return ssize_t The number of bytes we write, which returns -1 if the write operation fails.
 */
mlibc_weak ssize_t __mlibc_sys_write(int fd, void *buf, size_t buf_size)
{
    (void)fd;
    (void)buf;
    (void)buf_size;

    return -1;
}

#endif /* RT_USING_DFS */
#endif /* MLIBC_RUNNING_RT_THREAD */