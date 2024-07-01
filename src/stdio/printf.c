/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/28   0Bitbiscuits the first version
 */
#include "../internal/printf.h"
#include <compiler.h>
#include <stdarg.h>

mlibc_weak int printf(const char* format, ...)
{
    va_list va;
    int ret = 0;

    va_start(va, format);
    ret = printf_(format, va);
    va_end(va);

    return ret;
}