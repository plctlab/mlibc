/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/1    0Bitbiscuits the first version
 */
#include <stdio.h>
#include <stdarg.h>
#include <compiler.h>

mlibc_weak int fprintf(FILE* f, const char* format, ...)
{
    va_list va;
    int ret = 0;

    va_start(va, format);
    ret = vfprintf(f, format, va);
    va_end(va);

    return ret;
}