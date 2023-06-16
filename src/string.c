/*
 * Copyright (c) 2023
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/2/1       linshire     the first version
 * 2023/3/11      linshire     add memset
 */

#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>

void *memset(void *s, int c, size_t count)
{
    char *xs = (char *)s;

    while (count--)
        *xs++ = c;

    return s;
}

void *memcpy(void * restrict dst, const void * restrict src, size_t count)
{
    char *tmp = (char *)dst, *s = (char *)src;
    size_t len;

    if (tmp <= s || tmp > (s + count))
    {
        while (count--)
            *tmp ++ = *s ++;
    }
    else
    {
        for (len = count; len > 0; len --)
            tmp[len - 1] = s[len - 1];
    }

    return dst;
}

int memcmp(const void *cs, const void *ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for (su1 = (const unsigned char *)cs, su2 = (const unsigned char *)ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;

    return res;
}

void *memmove(void *d, const void *s, size_t n)
{
    char *dest = d;
    const char *src  = s;

    if ((size_t) (dest - src) < n)
    {
        /*
         * The <src> buffer overlaps with the start of the <dest> buffer.
         * Copy backwards to prevent the premature corruption of <src>.
         */

        while (n > 0)
        {
            n--;
            dest[n] = src[n];
        }
    }
    else
    {
        /* It is safe to perform a forward-copy */
        while (n > 0)
        {
            *dest = *src;
            dest++;
            src++;
            n--;
        }
    }

    return d;
}

void* memchr(const void* m, int c, size_t n)
{
    const unsigned char* s = m;
    c = (unsigned char)c;

    while(n--)
    {
        if(s[n] == c)
        {
            return (void*)(s + n);
        }
    }

    return 0;
}

size_t strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc) /* nothing */
        ;

    return sc - s;
}

int strcmp(const char *cs, const char *ct)
{
    while (*cs && *cs == *ct)
    {
        cs++;
        ct++;
    }

    return (*cs - *ct);
}

int strncmp(const char *cs, const char *ct, size_t count)
{
    register signed char __res = 0;

    while (count)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count --;
    }

    return __res;
}

char *strcpy(char *d, const char *s)
{
    char *dest = d;

    while (*s != '\0')
    {
        *d = *s;
        d++;
        s++;
    }

    *d = '\0';

    return dest;
}

char *strncpy(char *dst, const char *src, size_t n)
{
    if (n != 0)
    {
        char *d = dst;
        const char *s = src;

        do
        {
            if ((*d++ = *s++) == 0)
            {
                /* NUL pad the remaining n-1 bytes */
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }

    return (dst);
}

char *strcat(char * dest, const char * src)
{
    char *tmp = dest;

    while (*dest)
        dest++;
    while ((*dest++ = *src++) != '\0')
        ;

    return tmp;
}

char *strncat(char *dest, const char *src, size_t count)
{
    char *tmp = dest;

    if (count)
    {
        while (*dest)
            dest++;
        while ((*dest++ = *src++))
        {
            if (--count == 0)
            {
                *dest = '\0';
                break;
            }
        }
    }

    return tmp;
}

char *strrchr(const char* s, int c)
{
    return memchr(s, c, strlen(s) + 1);
}

char *strchr(const char* str, int c)
{
    while ((*str != 0) && (*str != c))
    {
        str++;
    }
    return ((*str == c) ? (char *)str : NULL);
}

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

char *__strchrnul(const char *s, int c)
{
    c = (unsigned char)c;
    if (!c) return (char *)s + strlen(s);

#ifdef __GNUC__
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % ALIGN; s++)
        if (!*s || *(unsigned char *)s == c) return (char *)s;
    size_t k = ONES * c;
    for (w = (void *)s; !HASZERO(*w) && !HASZERO(*w^k); w++);
    s = (void *)w;
#endif
    for (; *s && *(unsigned char *)s != c; s++);
    return (char *)s;
}

char *strstr(const char* string, const char* substring)
{
    const char* a;
    const char* b;

    /* First scan quickly through the two strings looking for a
     * single-character match.  When it's found, then compare the
     * rest of the substring.
     */

    b = substring;

    if(*b == 0)
    {
        return (char*)(uintptr_t)string;
    }

    for(; *string != 0; string += 1)
    {
        if(*string != *b)
        {
            continue;
        }

        a = string;

        while(1)
        {
            if(*b == 0)
            {
                return (char*)(uintptr_t)string;
            }

            if(*a++ != *b++)
            {
                break;
            }
        }

        b = substring;
    }

    return NULL;
}

char *strrev(char *str)
{
    assert(str != NULL);

    size_t len = strlen(str);
    size_t halflen = len >> 1;
    size_t i;
    for (i = 0; i < halflen; i++)
    {
        char tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
    return str;
}

int strcoll(const char* l, const char* r)
{
    return strcmp(l, r);
}

#define BITOP(a, b, op) \
    ((a)[(size_t)(b) / (8 * sizeof *(a))] op(size_t) 1 << ((size_t)(b) % (8 * sizeof *(a))))

size_t strcspn(const char* s, const char* c)
{
    const char* a = s;
    size_t byteset[32 / sizeof(size_t)];

    if(!c[0] || !c[1])
    {
        return (uintptr_t)(__strchrnul(s, *c) - (uintptr_t)a);
    }

    memset(byteset, 0, sizeof byteset);
    for(; *c && BITOP(byteset, *(const unsigned char*)c, |=); c++)
    {
        {
            ;
        }
    }
    for(; *s && !BITOP(byteset, *(const unsigned char*)s, &); s++)
    {
        {
            ;
        }
    }
    return (uintptr_t)s - (uintptr_t)a;
}

char* strtok_r(char* s, const char* delim, char** last)
{
    char* spanp;
    char* tok;
    int c;
    int sc;

    if(s == NULL && (s = *last) == NULL)
    {
        return (NULL);
    }

/*
 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
 */
cont:
    c = *s++;
    for(spanp = (char*)(uintptr_t)delim; (sc = *spanp++) != 0;)
    {
        if(c == sc)
        {
            goto cont;
        }
    }

    if(c == 0)
    { /* no non-delimiter characters */
        *last = NULL;
        return (NULL);
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for(;;)
    {
        c = *s++;
        spanp = (char*)(uintptr_t)delim;
        do
        {
            if((sc = *spanp++) == c)
            {
                if(c == 0)
                {
                    s = NULL;
                }
                else
                {
                    s[-1] = '\0';
                }
                *last = s;
                return (tok);
            }
        } while(sc != 0);
    }
    /* NOTREACHED */
}

char* strtok(char* s, const char* delim)
{
    static char* last;

    return (strtok_r(s, delim, &last));
}
