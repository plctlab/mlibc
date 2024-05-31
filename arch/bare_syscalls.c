/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/30   0Bitbiscuits the first version
 */
#include <unistd.h>
#include <fcntl.h>
#include <compiler.h>
#include <errno.h>

mlibc_weak void *sbrk(ptrdiff_t incr)
{
    extern char end;
    static char *heap_end = NULL;
    char *prev_heap_end = NULL;

    if(heap_end == 0)
    {
        heap_end = &end;
    }

    prev_heap_end = heap_end;
    heap_end += incr;

    return (void *)prev_heap_end;
}

mlibc_weak int open(const char *file, int flags, ...)
{
    (void)file;
    (void)flags;

    return -1;
}

mlibc_weak int creat(const char *path, mode_t mode)
{
    (void)path;
    (void)mode;

    return -1;
}

mlibc_weak int close(int d)
{
    (void)d;

    return -1;
}
mlibc_weak int read(int fd, void *buf, size_t len)
{
    (void)fd;
    (void)buf;
    (void)len;

    return -1;
}
mlibc_weak int fsync(int fildes)
{
    (void)fildes;

    return -1;
}

mlibc_weak ssize_t write(int fd, const void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;

    return -1;
}

mlibc_weak off_t lseek(int fd, off_t offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;

    return -1;
}
mlibc_weak int unlink(const char *pathname)
{
    (void)pathname;

    return -1;
}

mlibc_weak char *getcwd(char *buf, size_t size)
{
    (void)buf;
    (void)size;

    return NULL;
}
mlibc_weak int fcntl(int fd, int cmd, ... /* arg */ )
{
    (void)fd;
    (void)cmd;

    return -1;
}
mlibc_weak int ioctl(int fildes, int cmd, ...)
{
    (void)fildes;
    (void)cmd;

    return -1;
}

mlibc_weak int chdir(const char *path)
{
    (void)path;

    return -1;
}

mlibc_weak int mkdir(const char *pathname, mode_t mode)
{
    (void)pathname;
    (void)mode;

    return -1;
}

mlibc_weak int rmdir(const char *pathname)
{
    (void)pathname;

    return -1;
}

mlibc_weak int stat(const char *pathname, struct stat *statbuf)
{
    (void)pathname;
    (void)statbuf;

    return -1;
}
