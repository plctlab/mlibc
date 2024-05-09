/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/9    0Bitbiscuits  the first version
 */
#ifndef MLIBC_SYS_FILEOPS_H__
#define MLIBC_SYS_FILEOPS_H__

#include <rtconfig.h>

#ifdef RT_USING_DFS

#ifndef MLIBC_USING_FS
#define MLIBC_USING_FS
#endif /* MLIBC_USING_FS */

#include <dfs_file.h>

typedef struct dfs_file * mlibc_file_t;

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

#endif /* RT_USING_DFS */

#endif /* MLIBC_SYS_FILEOPS_H__ */
