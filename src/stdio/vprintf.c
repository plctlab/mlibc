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

mlibc_weak int vprintf(const char* format, va_list va)
{
    va_list vap;
    int ret = 0;

    va_copy(vap, vap);
    ret = vprintf_(format, vap);
    va_end(vap);
    return ret;
}