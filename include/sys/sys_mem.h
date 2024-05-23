/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/21   0BitBiscuits  the first version
 */
#ifndef PORTING_MEM_H__
#define PORTING_MEM_H__

#include <stddef.h>

// allocate memory from system
void* __mlibc_sbrk(ptrdiff_t increment);
// give memory back
void __mlibc_free(void *mem);

#endif /* PORTING_MEM_H__ */
