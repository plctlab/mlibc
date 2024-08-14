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
#include <stdlib.h>
#include <sys/types.h>
#include <mutex.h>

extern tlsf_t tlsf;
extern mutex_t heap_lock;
#define POOL_SIZE 1728                          /* size of pool head*/
#define LOCK_HEAP mutex_lock(&heap_lock, 0)     /* lock heap */
#define UNLOCK_HEAP mutex_unlock(&heap_lock) /* unlock heap */

void __mlibc_sys_heap_init(void);

#endif /* MLIBC_MEM_IMPL_H__*/
