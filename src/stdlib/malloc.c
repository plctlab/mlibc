/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <stdlib.h>
#include <assert.h>
#include <compiler.h>
#include "../internal/mem_impl.h"

#define MIN_MALLOC_FROM_SYS (1024)
#define MLIBC_ALIGN_SIZE    (16)
#define MLIBC_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

void *__malloc_r(size_t size)
{
    pool_t ret = NULL;
    void *block = NULL;
    size_t malloc_size = 0;
    size_t round = 0;
    size_t offset = 0;

    size = MLIBC_ALIGN(size, MLIBC_ALIGN_SIZE);
    
    LOCK_HEAP;
    block = tlsf_malloc(tlsf, size);
    if(block)
    {
        UNLOCK_HEAP;
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
    extern void *sbrk(int incr);
    block = sbrk(malloc_size);
    if(block != NULL)
    {
        if((ret = tlsf_add_pool(tlsf, block, malloc_size)) == NULL)
        {
            UNLOCK_HEAP;
            /* When memory allocation succeeds but fails to be added to the memory pool, return the allocated memory. */
            return NULL;
        }
        block = tlsf_malloc(tlsf, size);
    }
    UNLOCK_HEAP;
    
    return block;
}

mlibc_weak void *malloc(size_t size)
{
    return __malloc_r(size);
}