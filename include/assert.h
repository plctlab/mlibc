/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 * 2025/2/1      Chris-godz    the second version
 */

#include <compiler.h>

#undef assert

#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
mlibc_noreturn void __assert_fail(const char* expr, const char* file, int line);
mlibc_noreturn void __assert_func(const char *file, int line, const char *func, const char *failedexpr);

#define assert(expr) \
    ((expr) ? (void)0 : __assert_fail(#expr, __FILE__, __LINE__))
#endif
