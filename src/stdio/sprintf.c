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
#include <stdio.h>
#include <compiler.h>

mlibc_weak int sprintf(char* buffer, const char* format, ...)
{
    va_list va;
    int ret = 0;

    va_start(va, format);
    ret = vsnprintf(buffer, ((size_t)(-1) >> 1), format, va);
    va_end(va);

    return ret;
}