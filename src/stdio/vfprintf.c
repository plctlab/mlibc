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
    int ret = 0;

    va_copy(vap, va);
    FLOCK(f);
    ret = vsnprintf((char*)f->buf, BUFSIZ,  format, vap);
    f->write(f, f->buf, ret);
    FUNLOCK(f);
    va_end(vap);
    
    return ret;
}