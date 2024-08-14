/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <stdlib.h>
#include <compiler.h>
#include "../internal/mem_impl.h"

void __free_r(void* ptr)
{
   LOCK_HEAP;
   tlsf_free(tlsf, ptr);
   UNLOCK_HEAP;
}

mlibc_weak void free(void* ptr)
{
   __free_r(ptr);
}