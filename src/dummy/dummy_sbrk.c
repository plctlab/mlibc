/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/9/12  0Bitbiscuits the first version
 */
#include <stdio.h>
#include <compiler.h>

/**
 * @brief Memory allocation functions specifically implemented for
 * bare-metal systems (Currently supports only the GCC compiler)
 *  
 * @param incr allocate size
 * @return mlibc_weak* 
 */
mlibc_weak void *sbrk(int incr)
{
    extern int __bss_end;
    static int *heap_end = NULL;
    int *prev_heap_end = NULL;

    if(heap_end == 0)
    {
        heap_end = &__bss_end;
    }

    prev_heap_end = heap_end;
    heap_end += incr;

    return (void *)prev_heap_end;
}