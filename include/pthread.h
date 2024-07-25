/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/22   0Bitbiscuits the first version
 */
#ifndef MLIBC_PTHREAD_H__
#define MLIBC_PTHREAD_H__

#include <stdatomic.h>

#define pthread_mutexattr_t  void // not support pthread_mutexattr_t

typedef struct pthread_spinlock
{
    atomic_int lock;
} pthread_spinlock_t;

typedef void * __mutex_t;
typedef struct pthread_mutex
{
    __mutex_t sys_mutex;
} pthread_mutex_t;

int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int pthread_spin_destroy(pthread_spinlock_t *lock);
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
int pthread_spin_unlock(pthread_spinlock_t *lock);

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

#endif /* MLIBC_PTHREAD_H__ */
