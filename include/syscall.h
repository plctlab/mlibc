/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  the first version
 */
#ifndef MLIBC_SYSCALL_H__
#define MLIBC_SYSCALL_H__

#include <sys/types.h>

typedef struct
{
    void *buf;
    size_t buf_size;
} iovec_t;

/* Filesystem call*/
int __mlibc_sys_fcntl(int fd, int cmd, ...);
int __mlibc_sys_ioctl(int fd, int cmd, ...);
int __mlibc_sys_open(const char *path, int flags, ...);
int __mlibc_sys_close(int fd);
ssize_t __mlibc_sys_readv(int fd, iovec_t *iov, size_t iov_size);
ssize_t __mlibc_sys_read(int fd, unsigned char *buf, size_t buf_size);
ssize_t __mlibc_sys_writev(int fd, iovec_t *iov, size_t iov_size);
ssize_t __mlibc_sys_write(int fd, unsigned char *buf, size_t buf_size);
off_t __mlibc_sys_lseek(int fd, off_t offset, int whence);

#endif /* MLIBC_SYSCALL_H__ */
