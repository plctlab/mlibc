/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/22    0Bitbiscuits the first version
 */
#include <pthread.h>
#include <sys/errno.h>

int pthread_spin_init(pthread_spinlock_t *lock, int pshared)
{
    if (!lock)
        return EINVAL;

    atomic_store(&lock->lock, false);

    return 0;
}

int pthread_spin_destroy(pthread_spinlock_t *lock)
{
    if (!lock)
        return EINVAL;

    return 0;
}

int pthread_spin_lock(pthread_spinlock_t *lock)
{
    bool expected = false;
    while (!atomic_compare_exchange_weak_explicit(&lock->lock, &expected, true,
            memory_order_acquire, memory_order_relaxed)) {
        expected = false;
    }
    return 0;
}

int pthread_spin_trylock(pthread_spinlock_t *lock)
{
    bool expected = false;
    if (!atomic_compare_exchange_weak_explicit(&lock->lock, &expected, true,
        memory_order_acquire, memory_order_relaxed))
    {
        return 0;
    }
    return EBUSY;
}

int pthread_spin_unlock(pthread_spinlock_t *lock)
{
    atomic_store_explicit(&lock->lock, false, memory_order_release);

    return 0;
}