/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_UNISTD_H__
#define MLIBC_UNISTD_H__

#include <sys/types.h>

int close(int d);
int read(int fd, void *buf, size_t len);
int write(int fd, const void *buf, size_t len);
int fsync(int fildes);

#endif /*MLIBC_UNISTD_H__*/
