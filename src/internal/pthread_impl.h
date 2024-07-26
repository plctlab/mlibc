/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/25  0Bitbiscuits the first version
 */
#ifndef MLIBC_PTHREAD_MUTEX_IMPL_H__
#define MLIBC_PTHREAD_MUTEX_IMPL_H__

#include <pthread.h>

int __mutex_init(__mutex_t mutex);
int __mutex_destroy(__mutex_t mutex);
int __mutex_lock(__mutex_t mutex);
int __mutex_trylock(__mutex_t mutex);
int __mutex_unlock(__mutex_t mutex);

#endif /* MLIBC_PTHREAD_MUTEX_IMPL_H__ */