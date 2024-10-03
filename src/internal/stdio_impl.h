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

/* File lock ops */
#define FLOCK_INIT(f)   __lock_init_recursive(f->_flock)
#define FLOCK_DEINIT(f) __lock_deinit_recursive(f->_flock)
#define FLOCK(f)                                    \
    do{                                             \
        if(!f->_flock) __lock_init_recursive(f->_flock);    \
        __lock_take_recursive(f->_flock);           \
    }while(0)
#define FTRYLOCK(f)                                    \
    do{                                             \
        if(!f->_flock) __lock_init_recursive(f->_flock);    \
        __lock_trytake_recursive(f->_flock);           \
    }while(0)
#define FUNLOCK(f)      __lock_release_recursive(f->_flock)

#define STDOUT  stdout
#define STDIN   stdin
#define STDERR  stderr

/* Definition of FILE */
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
    char *getln_buf; // unused

    /* file list */
    FILE *prev, *next;

    /* file flags */
    unsigned char flags;

    /* file lock */
    _LOCK_RECURSIVE_T _flock;

    /* file offset */
    off_t off;
};

/* FILE ops */
int __mlibc_close(FILE *);
ssize_t __mlibc_read(FILE *, unsigned char *, size_t);
ssize_t __mlibc_write(FILE *, const unsigned char *, size_t);
off_t __mlibc_lseek(FILE *, off_t, int);
/* Init read/write buffer */
int __mlibc_toread(FILE *);
int __mlibc_towrite(FILE *);
/* Clean up the resources used by stdio(todo) */
void __mlibc_stdio_exit(void);
/* Change the mode(string) to file flags */
int __mlibc_fmode_to_flags(const char *mode);
/* FILE list operation */
FILE *__mlibc_file_add(FILE *f);
void __mlibc_file_remove(FILE *f);
FILE **__ofl_lock(void);
void __ofl_unlock(void);
/* The macro of getc/putc(unlocked) */
int __mlibc_uflow(FILE *f);
int __mlibc_overflow(FILE *f, int _c);
/* read and clean buffer */
#define __mlibc_getc_unlocked(f) \
    ( ((f)->rpos != (f)->rend) ? *(f)->rpos++ : __mlibc_uflow((f)) )
/* write and flush buffer */
#define __mlibc_putc_unlocked(c, f) \
    ( (((unsigned char)(c) != (f)->lbf && (f)->wpos != (f)->wend)) \
    ? *(f)->wpos++ = (unsigned char)(c) \
    : __mlibc_overflow((f),(unsigned char)(c)) )

#endif /* MLIBC_STDIO_IMPL_H__ */
