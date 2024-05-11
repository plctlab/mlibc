/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  the first version
 * 2024/5/9    0Bitbiscuits  add system type
 */
#ifndef MLIBC_SYSCALL_H__
#define MLIBC_SYSCALL_H__

#include <sys/types.h>
#include <sys_fileops.h>

/* system-related */
#ifdef MLIBC_USING_FS
typedef mlibc_file_t file_t;
#endif

#endif /* MLIBC_SYSCALL_H__ */

