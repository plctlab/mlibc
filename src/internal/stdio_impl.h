/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/6    0Bitbiscuits  the first version
 */
#ifndef MLIBC_STDIO_IMPL_H__
#define MLIBC_STDIO_IMPL_H__

#include <stdio.h>

typedef struct
{
    void *buf;
    size_t buf_size;
} iovec_t;

ssize_t __mlibc_close(FILE *);
ssize_t __mlibc_read(FILE *, unsigned char *, size_t);
ssize_t __mlibc_write(FILE *, unsigned char *, size_t);

off_t __mlibc_lseek(FILE *, off_t, int);

size_t __mlibc_toread(FILE *);
size_t __mlibc_towrite(FILE *);

void __mlibc_stdio_exit(void);

FILE *__mlibc_file_add(FILE *f);
void __mlibc_file_remove(FILE *f);
FILE *__mlibc_file_head(void);
int __mlibc_fmode_to_flags(const char *mode);

#endif /* MLIBC_STDIO_IMPL_H__ */
