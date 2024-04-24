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

typedef struct {
    int fd;
    char indicators;
} FILE;

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

int snprintf (char* buf, size_t buf_nbytes, const char* format, ...);
int sprintf (char* buf, const char* format, ...);
int printf (const char* format, ...);
int fprintf (FILE* stream, const char* format, ...);
int putchar(int c);
int fputc (int character, FILE* stream);
int putc (int character, FILE* stream);
int puts(const char* str);
int getc (FILE* stream);
int getchar (void);
int fgetc (FILE* stream);
int fileno(FILE *f);
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
