/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <compiler.h>
#include "../internal/mem_impl.h"

void *__realloc_r(void* ptr, size_t size)
{
    void* realloc_area = NULL;

    LOCK_HEAP;
    realloc_area = tlsf_realloc(tlsf, ptr, size);
    UNLOCK_HEAP;

    return realloc_area;
}

mlibc_weak void *realloc(void* ptr, size_t size)
{
    return __realloc_r(ptr, size);
}