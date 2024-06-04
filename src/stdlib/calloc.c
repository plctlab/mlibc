/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <string.h>
#include "../internal/mem_impl.h"
#include <compiler.h>

mlibc_weak void *calloc(size_t num, size_t size)
{
    void *block = NULL;

    block = malloc(num * size);
    if(block) memset(block, 0, num * size);

    return block;
}