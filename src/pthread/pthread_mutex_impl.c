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
#include <compiler.h>
#include <errno.h>

mlibc_weak int __mutex_init(__mutex_t mutex)
{
    mutex = NULL;
    return -EPERM;
}

mlibc_weak int __mutex_destroy(__mutex_t mutex)
{
    mutex = NULL;
    return -EPERM;
}

mlibc_weak int __mutex_lock(__mutex_t mutex)
{
    mutex = NULL;
    return -EPERM;
}

mlibc_weak int __mutex_trylock(__mutex_t mutex)
{
    mutex = NULL;
    return -EPERM;
}

mlibc_weak int __mutex_unlock(__mutex_t mutex)
{
    mutex = NULL;
    return -EPERM;
}