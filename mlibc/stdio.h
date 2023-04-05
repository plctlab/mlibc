#ifndef __STDIO_H__
#define __STDIO_H__

#include <typedef.h>
#include <stdarg.h>

#define EOF (-1)

struct __sbuf {
    unsigned char *_base;
    int _size;
};

typedef struct _iobuf
{
  unsigned char *_p;    /* current position in (some) buffer */
  int   _r;             /* read space left for getc() */
  int   _w;             /* write space left for putc() */
  short _flags;         /* flags, below; this FILE is free if 0 */
  short _file;          /* fileno, if Unix descriptor, else -1 */
  struct __sbuf _bf;    /* the buffer (at least 1 byte, if !NULL) */
  int   _lbfsize;       /* 0 or -_bf._size, for inline putc */
} FILE;

typedef _fpos_t     fpos_t;

#define ZEROPAD     (1 << 0)    /* pad with zero */
#define SIGN        (1 << 1)    /* unsigned/signed long */
#define PLUS        (1 << 2)    /* show plus */
#define SPACE       (1 << 3)    /* space if plus */
#define LEFT        (1 << 4)    /* left justified */
#define SPECIAL     (1 << 5)    /* 0x */
#define LARGE       (1 << 6)    /* use 'ABCDEF' instead of 'abcdef' */

#define SEEK_CUR     0x01
#define SEEK_END     0x02
#define SEEK_SET     0x03

int printf(const char *format, ...);
int vprintf(const char *format, va_list arg);
int fprintf(FILE *stream, const char *format, ...);
int vfprintf(FILE *stream, const char *format, va_list arg);

#endif /*#ifndef _STDIO_H_*/
