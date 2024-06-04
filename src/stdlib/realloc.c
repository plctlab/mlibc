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

mlibc_weak void *realloc(void* ptr, size_t size)
{
    return tlsf_realloc(tlsf, ptr, size);
}