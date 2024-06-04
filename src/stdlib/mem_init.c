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