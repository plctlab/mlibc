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

#include <sys/types.h>

// allocate memory from system
void __mlibc_sys_heap_init(void);
void* __mlibc_sys_malloc(size_t size);
void* __mlibc_sys_realloc(void *ptr, size_t size);
void __mlibc_sys_free(void *mem);

// heap controller
void *__mlibc_heap_sbrk(size_t size);
void __mlibc_heap_free(void *mem);

#endif /* PORTING_MEM_H__ */
