/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/17   0Bitbiscuits  the first version
 */
#include <tlsf.h>
#include <libc.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <libc_config.h>
#include <porting/porting_mem.h>
#include <assert.h>
#include <stdint.h>

#define MIN_MALLOC_FROM_SYS (1024 - tlsf_pool_overhead())

extern tlsf_t tlsf;

void *malloc(size_t size)
{
    size_t malloc_size = 0;
    pool_t ret = NULL;
    void *block = NULL;

    size = MLIBC_ALIGN(size, MLIBC_ALIGN_SIZE);
    if(block = tlsf_malloc(tlsf, size))
    {
        return block;
    }

    malloc_size = size < MIN_MALLOC_FROM_SYS ? MIN_MALLOC_FROM_SYS : size;
    if((block = __mlibc_sbrk(malloc_size + tlsf_pool_overhead())) != NULL)
    {
        if((ret = tlsf_add_pool(tlsf, block, malloc_size + tlsf_pool_overhead())) == NULL)
        {
            __mlibc_free(block);
        }
        block = tlsf_malloc(tlsf, size);
    }

    return block;
}

void *realloc(void* ptr, size_t size)
{
    if(ptr == NULL)
    {
        return malloc(size);
    }

    return tlsf_realloc(tlsf, ptr, size);
}

void *calloc(size_t num, size_t size)
{
    void *block = NULL;

    block = malloc(num * size);
    if(block) memset(block, 0, num * size);

    return block;
}

void free(void* ptr)
{
    tlsf_free(tlsf, ptr);
}