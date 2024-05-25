/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/17   0Bitbiscuits  the first version
 */
#include "../internal/tlsf.h"
#include <libc.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <libc_config.h>
#include <sys/sys_mem.h>
#include <assert.h>
#include <stdint.h>
#include <compiler.h>

#define MIN_MALLOC_FROM_SYS (1024 - tlsf_pool_overhead())

extern tlsf_t tlsf;

/* Ensure the allocated memory is greater than or equal to the requested size. */
mlibc_weak void *malloc(size_t size)
{
    pool_t ret = NULL;
    void *block = NULL;
    size_t malloc_size = 0;
    size_t round = 0;
    size_t offset = 0;

    size = MLIBC_ALIGN(size, MLIBC_ALIGN_SIZE);
    if(block = tlsf_malloc(tlsf, size))
    {
        return block;
    }

    malloc_size = size < MIN_MALLOC_FROM_SYS ? MIN_MALLOC_FROM_SYS : size;
    /* Additional space is needed when adding a memory block to the memory pool. */
    malloc_size += tlsf_pool_overhead();

    /* Optimizing the size of allocated memory blocks based on the TLSF algorithm mechanism. */
    offset = tlsf_fls_top(malloc_size) - tlsf_SL_INDEX_COUNT_LOG2();
    round = (1 << offset) - 1;
    malloc_size = (malloc_size + round) & ((~(1U)) << (offset - 1));

    /* Allocate memory blocks from system */
    if((block = __mlibc_sbrk(malloc_size)) != NULL)
    {
        if((ret = tlsf_add_pool(tlsf, block, malloc_size)) == NULL)
        {
            /* When memory allocation succeeds but fails to be added to the memory pool, return the allocated memory. */
            __mlibc_free(block);
        }
        block = tlsf_malloc(tlsf, size);
    }

    return block;
}

mlibc_weak void *realloc(void* ptr, size_t size)
{
    if(ptr == NULL)
    {
        return malloc(size);
    }

    return tlsf_realloc(tlsf, ptr, size);
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
    tlsf_free(tlsf, ptr);
}