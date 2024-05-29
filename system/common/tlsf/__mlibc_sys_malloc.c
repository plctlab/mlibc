/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/27  0Bitbiscuits the first version
 */
#include <libc_config.h>

#ifdef MLIBC_MEM_USING_TLSF

#include <sys/sys_mem.h>
#include <assert.h>
#include <compiler.h>
#include "inc/tlsf.h"

#define MIN_MALLOC_FROM_SYS (1024)

extern tlsf_t tlsf;

mlibc_weak void *__mlibc_sys_malloc(size_t size)
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
    if((block = __mlibc_heap_sbrk(malloc_size)) != NULL)
    {
        if((ret = tlsf_add_pool(tlsf, block, malloc_size)) == NULL)
        {
            /* When memory allocation succeeds but fails to be added to the memory pool, return the allocated memory. */
            __mlibc_heap_free(block);
        }
        block = tlsf_malloc(tlsf, size);
    }

    return block;
}

#endif /* MLIBC_MEM_USING_TLSF */