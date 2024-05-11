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

#define MLIBC_ALIGN_SIZE 8
#define MLIBC_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#define MLIBC_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

typedef int ptrdiff_t;

#endif /*MLIBC_STDDEF_H__*/
