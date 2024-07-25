/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/25  0Bitbiscuits the first version
 */
#include "../internal/pthread_impl.h"
#include <errno.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    if(mutex == NULL)
    {
        return 0;
    }
    attr = NULL;
    return __mutex_init(mutex->sys_mutex);
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    if(mutex == NULL)
    {
        return 0;
    }
    return __mutex_destroy(mutex->sys_mutex);
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    if(mutex == NULL)
    {
        return 0;
    }
    return pthread_mutex_lock(mutex->sys_mutex);
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    if(mutex == NULL)
    {
        return 0;
    }
    return pthread_mutex_trylock(mutex->sys_mutex);
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    if(mutex == NULL)
    {
        return 0;
    }
    return pthread_mutex_unlock(mutex->sys_mutex);
}