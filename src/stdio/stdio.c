/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/2/1       linshire     the first version
 * 2024/4/24      bitbiscuits  fix printf bug and realize snprintf
 */
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <compiler.h>
#include "../internal/stdio_impl.h"
/* Nedd to support reentrant(todo) */
mlibc_weak int putchar(int c)
{
    fputc(c, stdin);
    return c;
}

mlibc_weak int puts(const char* str)
{
    int r = 0;

    for(const char* c = str; *c != 0; c++)
    {
        putchar((int)*c);
        r++;
    }

    // puts adds a newline
    if (r)
    {
        putchar('\n');
        r++;
    }

    return r ? r : EOF;
}

mlibc_weak int fputc(int character, FILE* stream)
{
    return putc(character, stream);
}

mlibc_weak int putc (int character, FILE* stream)
{
    if(stream == NULL)
    {
        return EOF;
    }
    return write(stream->fd, &character, 1);
}

mlibc_weak int getc (FILE* stream)
{
    int buf = EOF;

    if(stream == NULL)
    {
        return EOF;
    }

    if(read(stream->fd, &buf, 1) == 1)
    {
        return buf;
    }
    else
    {
        return EOF;
    }
}

mlibc_weak int fgetc (FILE* stream)
{
    return getc(stream);
}

mlibc_weak int getchar(void)
{
    return fgetc(stdout);
}