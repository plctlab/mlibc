/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/29   0Bitbiscuits  realize sys_close
 */
#include <libc_config.h>
#include <sys/sys_fio.h>
#include <sys/sys_mem.h>
#include <errno.h>
#include <unistd.h>

#ifdef MLIBC_RUNNING_RTTHREAD
#include <rtconfig.h>
#include <rtthread.h>

#if defined(RT_USING_HEAP)

void *__mlibc_heap_sbrk(size_t size)
{
    return rt_malloc(size);
}

void __mlibc_heap_free(void *mem)
{
    rt_free(mem);
}

#endif /* RT_USING_HEAP */

#if defined(RT_USING_DFS) && defined(DFS_USING_POSIX)
#include <dfs_file.h>

int __mlibc_sys_close(int fd)
{
    close(fd);

    return 0;
}

int __mlibc_sys_fcntl(int fd, int cmd, ...)
{
    int res = 0;
    va_list args;

    va_start(args, cmd);
    res = fcntl(fd, cmd, args);
    va_end(args);

    return res;
}

int __mlibc_sys_ioctl(int fd, int cmd, ...)
{
    int res = 0;
    va_list args;

    va_start(args, cmd);
    res = ioctl(fd, cmd, args);
    va_end(args);

    return res;
}

off_t __mlibc_sys_lseek(int fd, off_t offset, int whence)
{
    return lseek(fd, offset, whence);
}

int __mlibc_sys_open(const char *path, int flags, ...)
{
    int ret = 0;
    va_list args;

    va_start(args, flags);
    ret = open(path, flags, args);
    va_end(args);

    return ret;
}

ssize_t __mlibc_sys_write(int fd, void *buf, size_t buf_size)
{
    ssize_t ret = 0;

    return write(fd, buf, buf_size);
}

ssize_t __mlibc_sys_read(int fd, void* buf, size_t buf_size)
{
    return read(fd, buf, buf_size);
}

#endif /* RT_USING_DFS && DFS_USING_POSIX */
#endif /* MLIBC_RUNNING_RT_THREAD */