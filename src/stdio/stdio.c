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
#include <assert.h>
#include <compiler.h>
#include "../internal/stdio_impl.h"

/* File put */
mlibc_weak int putchar(int c)
{
    fputc(c, stdout);
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

mlibc_weak int putc_unlocked(int character, FILE *stream)
{
    __mlibc_putc_unlocked(character, stream);
}

mlibc_weak int putchar_unlocked(int character)
{
    putc_unlocked(character, stdout);
}

mlibc_weak int putc (int character, FILE* stream)
{
    int ret = 0;

    FLOCK(stream);
    ret = putc_unlocked(character, stream);
    FUNLOCK(stream);

    return ret;
}

/* File put */
mlibc_weak int getc_unlocked(FILE *f)
{
    __mlibc_getc_unlocked(f);
}

mlibc_weak int getchar_unlocked(void)
{
    getc_unlocked(stdin);
}

mlibc_weak int getc (FILE* stream)
{
    int buf = EOF;

    FLOCK(stream);
    buf = getc_unlocked(stream);
    FUNLOCK(stream);

    return buf;
}

mlibc_weak int fgetc (FILE* stream)
{
    return getc(stream);
}

mlibc_weak int getchar(void)
{
    return fgetc(stdin);
}

/* File lock */
void flockfile(FILE *f)
{
    FLOCK(f);
}

void funlockfile(FILE *f)
{
    FUNLOCK(f);
}