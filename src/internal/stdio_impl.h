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

size_t __mlibc_open(int fd, const char *mode);
size_t __mlibc_close(FILE *);
size_t __mlibc_read(FILE *, unsigned char *, size_t);
size_t __mlibc_write(FILE *, const unsigned char *, size_t);
size_t __mlibc_seek(FILE *, off_t, int);

size_t __mlibc_toread(FILE *);
size_t __mlibc_towrite(FILE *);

void __mlibc_stdio_exit(void);

int __mlibc_lockfile(FILE *);
void __mlibc_unlockfile(FILE *);

#endif /* MLIBC_STDIO_IMPL_H__ */
