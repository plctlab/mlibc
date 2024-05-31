/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/30   0Bitbiscuits the first version
 */
#include <unistd.h>
#include <fcntl.h>
#include <compiler.h>
#include <rtthread.h>

void *sbrk (ptrdiff_t incr)
{
    void *blk = NULL;
#ifdef RT_USING_HEAP
    blk = rt_malloc(incr);
#endif
    return blk;
}
