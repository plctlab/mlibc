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

#ifdef MLIBC_RUNNING_BARE

#include <sys/sys_mem.h>
#include <assert.h>
#include <compiler.h>

#define HEAP_DEFAULT_SIZE   1024 * 10
#define MLIBC_HEAP_START    (&MLIBC_HEAP[0])
#define MLIBC_HEAP_END      (&MLIBC_HEAP[HEAP_DEFAULT_SIZE - 1])

static char MLIBC_HEAP[HEAP_DEFAULT_SIZE];
static char *cur_point = MLIBC_HEAP_START;
static char *prev_point = NULL;

/**
 * @brief Here is a basic implementation of system memory 
 * allocation in a bare-metal state.If you have better ideas, 
 * you can override this function or even modify this source 
 * file to implement your optimizations.
 * 
 * @param size The size of the memory block you want to allocate.
 * @return void* The address of the target memory block.
 */
mlibc_weak void *__mlibc_heap_sbrk(size_t size)
{
    if(cur_point == NULL)
    {
        cur_point = MLIBC_HEAP_START;
    }

    if((size < 0) || (cur_point + size > MLIBC_HEAP_END) || (cur_point + size < MLIBC_HEAP_START))
    {
        return NULL;
    }

    prev_point = cur_point;
    cur_point += size;

    return (void *)prev_point;
}

/**
 * @brief In a bare-metal state, system memory release function is 
 * temporarily not implemented by default. If you have a better memory
 * management method, you can override this function or rewrite this 
 * source file to implement it.
 * 
 * @param mem The memory block address you allocated
 * @return void
 */
mlibc_weak void __mlibc_heap_free(void *mem)
{
    assert(0);
}

#endif /* MLIBC_RUNNING_BARE */