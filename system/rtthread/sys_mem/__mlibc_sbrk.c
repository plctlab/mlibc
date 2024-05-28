/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/23   0Bitbiscuits  the first version
 */
#include <libc_config.h>
#ifdef MLIBC_RUNNING_RTTHREAD

#include <sys/sys_mem.h>
#include <rtthread.h>

void *__mlibc_heap_sbrk(size_t size)
{
    return rt_malloc(size);
}

void __mlibc_heap_free(void *mem)
{
    rt_free(mem);
}

#endif /* MLIBC_RUNNING_RTTHREAD */