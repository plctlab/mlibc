/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */
#ifndef MLIBC_STDIO_H__
#define MLIBC_STDIO_H__
#include <sys/types.h>
#include <stdarg.h>

#define EOF (-1)

#define _FILE_IND_EOF   (1 << 0)
#define _FILE_IND_ERROR (1 << 1)

struct __MLIBC_IO_FILE;
typedef struct __MLIBC_IO_FILE FILE;
struct __MLIBC_IO_FILE{
    int fd;
    unsigned char *wbase;
    unsigned char *wpos, wend;
    unsigned char *rpos, rend;
    size_t (*write)(FILE *, unsigned char *, size_t);
    size_t (*read)(FILE *, unsigned char *, size_t);
    off_t (*seek)(FILE *, off_t, int);

    unsigned char *buf;
    size_t buf_size;

    int lbf;
    char *getln_buf;

    FILE *prev, *next;

    volatile int lock;
    long lockcount;
    FILE *prev_lock, *next_lock;

    int mode;
    off_t off;
    unsigned char indicators;
};

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

#define BUFSIZ 128
#define EOF     (-1)

#ifndef SEEK_SET
#define SEEK_SET    0   /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR    1   /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END    2   /* set file offset to EOF plus offset */
#endif

#define CHAR_BIT 8
#define UCHAR_MAX 255

/* File open and close */
int fclose(FILE *);
FILE *fdopen(int, const char *);
int fopen(const char *, const char *);
FILE *freopen(const char *, const char *, FILE *);

/* Formatted I/O status */
int printf(const char *, ...);
void perror(const char *);
int scanf(const char *, ...);
int snprintf(char *, size_t, const char *, ...);
int sprintf(char *, const char *, ...);
int sscanf(const char *, const char *, ...);
int vfprintf(FILE *, const char *, va_list);
int vfscanf(FILE *, const char *, va_list);
int vprintf(const char *, va_list);
int vscanf(const char *, va_list);
int vsnprintf(char *, size_t, const char *, va_list);
int vsprintf(char *, const char *, va_list);
int vsscanf(const char *, const char *, va_list);

/* File read and write operations */
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
char *fgets(char *, int, FILE *);
int fileno(FILE *);
void flockfile(FILE *);
int fprintf(FILE *, const char *, ...);
int fputc(int, FILE *);
int fputs(const char *, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc(FILE *);
int getc_unlocked(FILE *);
int getchar(void);
int getchar_unlocked(void);
char *gets(char *);
int putc(int, FILE *);
int putc_unlocked(int, FILE *);
int putchar(int);
int putchar_unlocked(int);
int puts(const char *);
int ungetc(int, FILE *);


int libc_stdio_set_console(const char* device_name, int mode);
int libc_stdio_get_console(void);

extern char **__environ;

void __env_rm_add(char *old, char *new);
int __putenv(char *s, size_t l, char *r);
int putenv(char *s);
char *getenv(const char *name);
int setenv(const char *var, const char *value, int overwrite);
int unsetenv(const char *name);

int rename(const char *oldpath, const char *newpath);

#endif /*MLIBC_STDIO_H__*/
