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
#include "../internal/mem_impl.h"

tlsf_t tlsf;
_LOCK_T heap_lock;
static char libc_heap[POOL_SIZE];

static tlsf_t __heap_init(void *mem, size_t size)
{
    assert(size >= tlsf_size() && "Need more memory to init heap management\n");
    
    return tlsf_create(mem);
}

/* Initialize mlibc memory heap */
mlibc_weak void __mlibc_sys_heap_init(void)
{   
    if(!tlsf)
    {
        assert((INIT_HEAP_LOCK == 0) && "Heap lock init failed\n");
        tlsf = __heap_init(libc_heap, POOL_SIZE);
        assert(tlsf != NULL && "Heap init failed\n");
    }
}