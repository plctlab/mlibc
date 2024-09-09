/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021/02/17     Bernard      first version
 */

#ifndef MLIBC_STDDEF_H__
#define MLIBC_STDDEF_H__

#ifdef __cplusplus
    #define NULL (0)
#else
    #define NULL ((void*)0)
#endif /*__cplusplus*/

#define offsetof(type, member) ((size_t) &((type *)0)->member)

#if (defined(__GNUC__) && (__SIZEOF_POINTER__ == 8))
typedef signed long long    ptrdiff_t;
#else
typedef signed int          ptrdiff_t;
#endif /* __GNUC__ */

#endif /*MLIBC_STDDEF_H__*/
