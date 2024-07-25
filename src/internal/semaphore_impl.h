/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/25  0Bitbiscuits the first version
 */
#ifndef MLIBC_SEMAPHORE_IMPL_H__
#define MLIBC_SEMAPHORE_IMPL_H__

#include <semaphore.h>

#ifndef NULL
#define NULL (void*)0
#endif

int __sem_init(__sem_t sem, unsigned int value);
int __sem_wait(__sem_t sem);
int __sem_trywait(__sem_t sem);
int __sem_post(__sem_t sem);
// int __sem_timedwait(sem_t *sem, const struct timespec *abs_timeout); todo
int __sem_destroy(__sem_t sem);

#endif /* MLIBC_SEMAPHORE_IMPL_H__ */