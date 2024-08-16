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

#define FLOCK(f)
#define FUNLOCK(f)

typedef struct
{
    void *buf;
    size_t buf_size;
} iovec_t;

struct __MLIBC_IO_FILE{
    int fd;
    unsigned char *wbase;
    unsigned char *wpos, *wend;
    unsigned char *rpos, *rend;
    ssize_t (*write)(FILE *, unsigned char *, size_t);
    ssize_t (*read)(FILE *, unsigned char *, size_t);
    off_t (*seek)(FILE *, off_t, int);
    ssize_t (*close)(FILE *);

    /* buffer */
    unsigned char *buf;
    size_t buf_size;

    /* line buffered mode */
    int lbf;
    char *getln_buf;

    /* file list */
    FILE *prev, *next;

    /* file mode */
    int mode;

    /* file lock */
    
    /* file offset */
    off_t off;
    unsigned char flags;
};

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
