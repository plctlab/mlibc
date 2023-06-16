/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_ASSERT_H__
#define MLIBC_ASSERT_H__

#ifdef NDEBUG
#define assert(expr)((void) 0)
#else
void __assert_fail (const char* expr, const char* file, int line);

#define assert(expr) \
    if (!(expr)) \
        __assert_fail(#expr, __FILE__, __LINE__)
#endif

#endif /*MLIBC_ASSERT_H__*/

