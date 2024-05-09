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
#include <sys_thread.h>

/* system-related */
#ifdef MLIBC_USING_FS
typedef mlibc_file_t file_t;
#endif

#ifdef MLIBC_USING_THREAD
typedef mlibc_timer_t timer_t;
typedef mlibc_thread_t thread_t;

#ifdef MLIBC_USING_MUTEX
typedef mlibc_mutex_t mutex_t;
#endif /* MLIBC_USING_MUTEX */

#ifdef MLIBC_USING_SEMAPHORE
typedef mlibc_semaphore_t sem_t;
#endif /* MLIBC_USING_SEMAPHORE */

#endif /* MLIBC_USING_THREAD */

#endif /* MLIBC_SYSCALL_H__ */
