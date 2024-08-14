/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/8   0Bitbiscuits the first version
 */
#ifndef MLIBC_MUTEX_H__
#define MLIBC_MUTEX_H__

#include <stdint.h>

struct _mutex
{
    void* sys_mutex;
};
typedef struct _mutex mutex_t;

int mutex_init(mutex_t *mutex);
int mutex_deinit(mutex_t *mutex);
int mutex_lock(mutex_t *mutex, uint32_t timeout);
int mutex_trylock(mutex_t *mutex);
int mutex_unlock(mutex_t *mutex);

#endif /* MLIBC_MUTEX_H__ */