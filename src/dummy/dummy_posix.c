/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/9/3   0Bitbiscuits the first version
 */
#include <unistd.h>
#include <compiler.h>
#include <errno.h>
#include <fcntl.h>

mlibc_weak ssize_t read(int fd, void *buf, size_t len)
{
    (void)fd;
    (void)buf;
    (void)len;

    return -EPERM;
}

mlibc_weak off_t lseek(int fd, off_t offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;

    return -EPERM;
}

mlibc_weak ssize_t write(int fd, const void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;

    return -EPERM;
}

mlibc_weak int open(const char *file, int flags, ...)
{
    (void)file;
    (void)flags;

    return -EPERM;
}

mlibc_weak int close(int d)
{
    (void)d;

    return -EPERM;
}

mlibc_weak int fcntl(int fd, int cmd, ... /* arg */ )
{
    (void)fd;
    (void)cmd;

    return -EPERM;
}

mlibc_weak int ioctl(int fildes, int cmd, ...)
{
    (void)fildes;
    (void)cmd;

    return -EPERM;
}

mlibc_weak int raise(int sig)
{
    (void)sig;
    return -EPERM;
}