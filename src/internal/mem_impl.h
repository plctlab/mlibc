/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/14  0Bitbiscuits the first version
 */
#ifndef MLIBC_MEM_IMPL_H__
#define MLIBC_MEM_IMPL_H__

#include "tlsf.h"
#include "lock.h"
#include <stdlib.h>
#include <sys/types.h>

extern tlsf_t tlsf;
extern _LOCK_T heap_lock;

#define POOL_SIZE   1728                          /* size of pool head*/

#define LOCK_HEAP   __lock_take(heap_lock)        /* lock heap */
#define UNLOCK_HEAP __lock_release(heap_lock)   /* unlock heap */

void __mlibc_sys_heap_init(void);

#endif /* MLIBC_MEM_IMPL_H__*/
