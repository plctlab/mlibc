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
#include <assert.h>
#include <stdint.h>

extern tlsf_t tlsf;

void __mlibc_heap_nosys_init(void *begin_addr, void *end_addr)
{
    ssize_t begin_align = MLIBC_ALIGN((ssize_t)begin_addr, MLIBC_ALIGN_SIZE);
    ssize_t end_align   = MLIBC_ALIGN_DOWN((ssize_t)end_addr, MLIBC_ALIGN_SIZE);

    assert(end_align > begin_align);

    /* Initialize mlibc memory heap */
    tlsf = tlsf_create_with_pool((void *)begin_align, end_align - begin_align);
}

void __mlibc_heap_sys_init(void)
{
    /* Initialize mlibc memory heap */
    void *block = NULL;

    block = sys_malloc(MLIBC_ALIGN(tlsf_control_size(), MLIBC_ALIGN_SIZE));
    tlsf = tlsf_create(block);
}

void *malloc(size_t size)
{
    void *block = NULL;

    if(block = tlsf_malloc(tlsf, size))
    {
        return block;
    }

    if(mlibc.RUN_IN_OS)
    {
        block = sys_malloc(size + tlsf_pool_overhead());
        if(block)
        {
            tlsf_add_pool(tlsf, block, size + tlsf_pool_overhead());
            block = tlsf_malloc(tlsf, size);
        }
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