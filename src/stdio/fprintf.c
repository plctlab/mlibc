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
#include "../internal/stdio_impl.h"
#include <errno.h>
#include <stdio.h>

int fprintf(FILE* f, const char* format, ...)
{
    va_list va;
    char f_buffer[BUFSIZ];

    if(f == NULL)
    {
        return -EINVAL;
    }

    va_start(va, format);
    int ret = vsnprintf_(f_buffer, BUFSIZ,  format, va);
    f->write(f, f_buffer, ret);
    va_end(va);
    return ret;
}