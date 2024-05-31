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
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <compiler.h>
#include "../internal/tlsf.h"
#include "../internal/mem_impl.h"

#define MIN_MALLOC_FROM_SYS (1024)

tlsf_t tlsf;

static tlsf_t __heap_init(void *mem, size_t size)
{
    assert(size >= tlsf_size() && "Need more memory to init heap management");
    
    return tlsf_create(mem);
}

/* Initialize mlibc memory heap */
mlibc_weak void __mlibc_sys_heap_init(void)
{
    void *ret = NULL;

    if(!tlsf)
    {
        ret = sbrk(tlsf_size());
        assert(ret && "memory controller init failed");
        tlsf = __heap_init(ret, tlsf_size());
    }
}

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
    if((block = sbrk(malloc_size)) != NULL)
    {
        if((ret = tlsf_add_pool(tlsf, block, malloc_size)) == NULL)
        {
            /* When memory allocation succeeds but fails to be added to the memory pool, return the allocated memory. */
            return NULL;
        }
        block = tlsf_malloc(tlsf, size);
    }

    return block;
}

mlibc_weak void *realloc(void* ptr, size_t size)
{
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