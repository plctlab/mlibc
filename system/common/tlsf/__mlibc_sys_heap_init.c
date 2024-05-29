/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/27   0Bitbiscuits  the first version
 */
#include <libc_config.h>

#ifdef MLIBC_MEM_USING_TLSF

#include <sys/sys_mem.h>
#include <assert.h>
#include <compiler.h>
#include "inc/tlsf.h"

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
        ret = __mlibc_heap_sbrk(tlsf_size());
        assert(ret && "memory controller init failed");
        tlsf = __heap_init(ret, tlsf_size());
    }
}

#endif /* MLIBC_MEM_USING_TLSF */