/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/9/3   0Bitbiscuits the first version
 */
#include <compiler.h>
#include "../internal/lock.h"

struct __lock
{
    void* dummy_lock;
};

mlibc_weak int __retarget_lock_init(_LOCK_T *lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_init_recursive(_LOCK_T *lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_deinit(_LOCK_T lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_deinit_recursive(_LOCK_T lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_take(_LOCK_T lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_take_recursive(_LOCK_T lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_trytake(_LOCK_T lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_trytake_recursive(_LOCK_T lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_release(_LOCK_T lock)
{
    return 0;
}

mlibc_weak int __retarget_lock_release_recursive(_LOCK_T lock)
{
    return 0;
}
