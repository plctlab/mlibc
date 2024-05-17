/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/17   0Bitbiscuits  the first version
 */
#ifndef MLIBC_SYS_MEM_H__
#define MLIBC_SYS_MEM_H__

#ifdef RT_USING_HEAP

#ifndef MLIBC_USING_HEAP
#define MLIBC_USING_HEAP
#endif /* MLIBC_USING_HEAP */

#define sys_malloc(size) rt_malloc(size)
#define sys_free(size) rt_free(size)
#define sys_realloc(ptr, size) rt_realloc(ptr, size)

#else

#define sys_malloc(size) (void *)0
#define sys_free(ptr)
#define sys_realloc(ptr, size) (void *)0

#endif /* RT_USING_HEAP */

#endif /* MLIBC_SYS_MEM_H__ */
