/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/25  0Bitbiscuits the first version
 */
#include "../internal/semaphore_impl.h"
#include <compiler.h>
#include <errno.h>

mlibc_weak int __sem_init(__sem_t sem, unsigned int value)
{
    sem = NULL;
    return -EPERM;
}

mlibc_weak int __sem_destroy(__sem_t sem)
{
    sem = NULL;
    return -EPERM;
}

mlibc_weak int __sem_wait(__sem_t sem)
{
    sem = NULL;
    return -EPERM;
}

mlibc_weak int __sem_trywait(__sem_t sem)
{
    sem = NULL;
    return -EPERM;
}

mlibc_weak int __sem_post(__sem_t sem)
{
    sem = NULL;
    return -EPERM;
}