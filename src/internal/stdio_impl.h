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
#include "lock.h"

#define FLOCK_INIT(f)   __lock_init(f->_flock)
#define FLOCK_DEINIT(f) __lock_deinit(f->_flock)
#define FLOCK(f)        __lock_take(f->_flock)
#define FUNLOCK(f)      __lock_release(f->_flock)

struct __MLIBC_IO_FILE{
    int fd;

    unsigned char *wbase;
    unsigned char *wpos, *wend;
    unsigned char *rpos, *rend;

    ssize_t (*write)(FILE *, const unsigned char *, size_t);
    ssize_t (*read)(FILE *, unsigned char *, size_t);
    off_t (*seek)(FILE *, off_t, int);
    int (*close)(FILE *);

    /* buffer */
    unsigned char *buf;
    size_t buf_size;

    /* line buffered mode */
    int lbf;
    char *getln_buf;

    /* file list */
    FILE *prev, *next;

    /* file flags */
    unsigned char flags;

    /* file lock */
    _LOCK_RECURSIVE_T _flock;

    /* file offset */
    off_t off;
};

int __mlibc_close(FILE *);
ssize_t __mlibc_read(FILE *, unsigned char *, size_t);
ssize_t __mlibc_write(FILE *, const unsigned char *, size_t);

off_t __mlibc_lseek(FILE *, off_t, int);

size_t __mlibc_toread(FILE *);
size_t __mlibc_towrite(FILE *);

void __mlibc_stdio_exit(void);

FILE *__mlibc_file_add(FILE *f);
void __mlibc_file_remove(FILE *f);
FILE **__ofl_lock(void);
void __ofl_unlock(void);
int __mlibc_fmode_to_flags(const char *mode);

#endif /* MLIBC_STDIO_IMPL_H__ */
