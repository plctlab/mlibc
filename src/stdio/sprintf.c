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

mlibc_weak int sprintf_(char* buffer, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    const int ret = sprintf_(buffer, format, va);
    va_end(va);
    return ret;
}