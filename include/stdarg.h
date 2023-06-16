/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_STDARG_H__
#define MLIBC_STDARG_H__

#define va_list     __builtin_va_list
#define va_start    __builtin_va_start
#define va_end      __builtin_va_end
#define va_arg      __builtin_va_arg
#define va_copy     __builtin_va_copy

#endif
