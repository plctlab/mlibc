/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/9    0Bitbiscuits  the first version
 */
#ifndef MLIBC_SYS_THREAD_H__
#define MLIBC_SYS_THREAD_H__

#include <rtconfig.h>

#ifndef MLIBC_USING_THREAD
#define MLIBC_USING_THREAD
#endif /* MLIBC_USING_THREAD */

#include <rtdef.h>

typedef struct rt_thread * mlibc_thread_t;
typedef struct rt_timer * mlibc_timer_t;

/* RT_USING_MUTEX */
#ifdef RT_USING_MUTEX
#ifndef MLIBC_USING_MUTEX
#define MLIBC_USING_MUTEX
#endif /* MLIBC_USING_MUTEX */

typedef struct rt_mutex * mlibc_mutex_t;
#endif /* RT_USING_MUTEX */

/* RT_USING_SEMAPHORE */
#ifdef RT_USING_SEMAPHORE
#ifndef MLIBC_USING_SEMAPHORE
#define MLIBC_USING_SEMAPHORE
#endif /* MLIBC_USING_SEMAPHORE */

typedef struct rt_semaphore * mlibc_semaphore_t;
#endif /* RT_USING_SEMAPHORE */

#endif /* MLIBC_SYS_THREAD_H__ */
