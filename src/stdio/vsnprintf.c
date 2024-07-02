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
#include <stdarg.h>
#include <compiler.h>

mlibc_weak int vsnprintf(char* buffer, size_t count, const char* format, va_list va)
{
    va_list vap;
    int ret = 0;

    va_copy(vap, va);
    ret = vsnprintf_(buffer, count, format, vap);
    va_end(vap);

    return ret;
}