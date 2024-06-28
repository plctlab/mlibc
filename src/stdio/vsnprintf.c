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


int vsnprintf(char* buffer, size_t count, const char* format, va_list va)
{
    va_list va;
    va_start(va, format);
    const int ret = vsnprintf_(buffer, count, format, va);
    va_end(va);
    return ret;
}