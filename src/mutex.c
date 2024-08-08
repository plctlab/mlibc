/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/8   0Bitbiscuits the first version
 */
#include <mutex.h>
#include <compiler.h>
#include <errno.h>

#ifndef NULL
#define NULL (void *)0
#endif

mlibc_weak int mutex_init(mutex_t *mutex)
{
    /* Assign the system's mutex to sys_mutex */
    mutex->sys_mutex = NULL;
    return -EPERM;
}

mlibc_weak int mutex_deinit(mutex_t *mutex)
{
    /* Setting this value to null 
    ** means you can no longer use it
    */
    mutex->sys_mutex = NULL;
    return -EPERM;
}

/**
 * @brief Request to acquire a lock
 * 
 * @param mutex Initialized mutex pointer
 * @param timeout Maximum waiting time for the lock.
 * @return mlibc_weak The result of the request
 */
mlibc_weak int mutex_lock(mutex_t *mutex, uint32_t timeout)
{
    /* Request to acquire a lock with a specified timeout */
    (void)mutex;
    (void)timeout;
    return -EPERM;
}

/**
 * @brief Try to request for the lock
 * 
 * @param mutex Initialized mutex pointer
 * @return mlibc_weak The result of the request
 */
mlibc_weak int mutex_trylock(mutex_t *mutex)
{
    /* Request to acquire a lock
    ** if the lock is busy then exit
    */
    (void)mutex;
    return -EPERM;
}

/**
 * @brief Release the lock
 * 
 * @param mutex The mutex you want to release
 */
mlibc_weak int mutex_unlock(mutex_t *mutex)
{
    /* Call system mutex unlock function */
    (void)mutex;
    return -EPERM;
}