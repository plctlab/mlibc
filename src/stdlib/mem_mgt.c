/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/17   0Bitbiscuits  the first version
 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sys_mem.h>
#include <compiler.h>

mlibc_weak void *malloc(size_t size)
{
    if(size == 0) return NULL;

    return __mlibc_sys_malloc(size);
}

mlibc_weak void *realloc(void* ptr, size_t size)
{
    void *ret = NULL;
    if(ptr && size == 0)
    {
        free(ptr);
    }
    else if(!ptr)
    {
        ret = malloc(size);
    }
    else
    {
        ret = __mlibc_sys_realloc(ptr, size);
    }

    return ret;
}

mlibc_weak void *calloc(size_t num, size_t size)
{
    void *block = NULL;

    block = malloc(num * size);
    if(block) memset(block, 0, num * size);

    return block;
}

mlibc_weak void free(void* ptr)
{
    if(ptr == NULL) return;

    __mlibc_sys_free(ptr);
}