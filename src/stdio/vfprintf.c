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
#include <stdarg.h>
#include <compiler.h>

mlibc_weak int vfprintf(FILE *f, const char *format, va_list va)
{
    va_list vap;
    char f_buffer[BUFSIZ];
    int ret;

    if(f == NULL)
    {
        return -EINVAL;
    }

    va_copy(vap, va);
    ret = vsnprintf_(f_buffer, BUFSIZ,  format, vap);
    f->write(f, f_buffer, ret);
    va_end(vap);
    
    return ret;
}