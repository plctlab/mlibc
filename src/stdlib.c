/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>

double atof(const char *str)
{
    double s=0.0;

    double d=10.0;
    int jishu=0;

    bool flag=false;

    while(*str==' ')
    {
        str++;
    }

    if(*str=='-')
    {
        flag=true;
        str++;
    }

    if(!(*str>='0'&&*str<='9'))
        return s;

    while(*str>='0'&&*str<='9'&&*str!='.')
    {
        s=s*10.0+*str-'0';
        str++;
    }

    if(*str=='.')
        str++;

    while(*str>='0'&&*str<='9')
    {
        s=s+(*str-'0')/d;
        d*=10.0;
        str++;
    }

    if(*str=='e'||*str=='E')
    {
        str++;
        if(*str=='+')
        {
            str++;
            while(*str>='0'&&*str<='9')
            {
                jishu=jishu*10+*str-'0';
                str++;
            }
            while(jishu>0)
            {
                s*=10;
                jishu--;
            }
        }
        if(*str=='-')
        {
            str++;
            while(*str>='0'&&*str<='9')
            {
                jishu=jishu*10+*str-'0';
                str++;
            }
            while(jishu>0)
            {
                s/=10;
                jishu--;
            }
        }
    }

    return s*(flag?-1.0:1.0);
}

int atoi(const char *str)
{
    int i = 0;
    int nega = 0;

    while (isspace(*str))
    {
        str++;
    }
    switch (*str)
    {
    case '-':
        nega = 1;
        str++;
        break;
    case '+':
        str++;
    }
    while (isdigit(*str))
    {
        i = 10*i - (*str++ - '0');
    }
    return nega ? i : -i;
}

long atol(const char *s)
{
    long n=0;
    int neg=0;

    while (isspace(*s)) s++;
    switch (*s)
    {
    case '-':
        neg = 1;
        s++;
        break;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on LONG_MIN */
    while (isdigit(*s))
        n = 10*n - (*s++ - '0');
    return neg ? n : -n;
}

long long atoll(const char *s)
{
    long long n=0;
    int neg=0;

    while (isspace(*s)) s++;
    switch (*s)
    {
    case '-':
        neg=1;
        s++;
        break;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on LLONG_MIN */
    while (isdigit(*s))
        n = 10*n - (*s++ - '0');
    return neg ? n : -n;
}

div_t div(int num, int den)
{
    return (div_t){ num/den, num%den };
}

long labs(long a)
{
    return a>0 ? a : -a;
}

ldiv_t ldiv(long num, long den)
{
    return (ldiv_t){ num/den, num%den };
}

long long llabs(long long a)
{
    return a>0 ? a : -a;
}

lldiv_t lldiv(long long num, long long den)
{
    return (lldiv_t){ num/den, num%den };
}

float strtof(char *str, char **ptr)
{
    return (float)strtod(str, ptr);
}

double strtod(char *str, char **ptr)
{
    char *p;
    if (ptr == (char **)0)
        return atof(str);

    p = str;

    while(isspace(*p))
        ++p;

    if(*p == '+' || *p == '-')
        ++p;
    /* INF or INFINITY.  */
    if((p[0] == 'i' || p[0] == 'I')
        && (p[1] == 'n' || p[1] == 'N')
        && (p[2] == 'f' || p[2] == 'F'))
    {
        if((p[3] == 'i' || p[3] == 'I')
            && (p[4] == 'n' || p[4] == 'N')
            && (p[5] == 'i' || p[5] == 'I')
            && (p[6] == 't' || p[6] == 'T')
            && (p[7] == 'y' || p[7] == 'Y'))
        {
            *ptr = p + 8;
            return atof(str);
        }
        else
        {
            *ptr = p + 3;
            return atof(str);
        }
    }
    /* NAN or NAN(foo).  */
    if((p[0] == 'n' || p[0] == 'N')
        && (p[1] == 'a' || p[1] == 'A')
        && (p[2] == 'n' || p[2] == 'N'))
    {
        p += 3;
        if(*p == '(')
        {
            ++p;
            while(*p != '\0' && *p != ')')
                ++p;
            if (*p == ')')
                ++p;
        }
        *ptr = p;
        return atof(str);
    }
    /* digits, with 0 or 1 periods in it.  */
    if(isdigit(*p) || *p == '.')
    {
        int got_dot = 0;
        while(isdigit(*p) || (!got_dot && *p == '.'))
        {
            if(*p == '.')
                got_dot = 1;
            ++p;
        }
        /* Exponent.  */
        if(*p == 'e' || *p == 'E')
        {
            int i;
            i = 1;
            if(p[i] == '+' || p[i] == '-')
                ++i;
            if(isdigit(p[i]))
            {
                while(isdigit(p[i]))
                    ++i;
                *ptr = p + i;
                return atof(str);
            }
        }
        *ptr = p;
        return atof(str);
    }
    /* Didn't find any digits.  Doesn't look like a number.  */
    *ptr = str;
    return 0.0;
}

long double strtold(char *str, char **ptr)
{
    return (long double)strtod(str, ptr);
}

long strtol(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    unsigned long acc;
    int c;
    unsigned long cutoff;
    int neg = 0, any, cutlim;
    /*
        * Skip white space and pick up leading +/- sign if any.
        * If base is 0, allow 0x for hex and 0 for octal, else
        * assume decimal; if base is already 16, allow 0x.
        */
    do
    {
        c = *s++;
    } while(isspace(c));
    if(c == '-')
    {
        neg = 1;
        c = *s++;
    }
    else if(c == '+')
        c = *s++;
    if((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X'))
    {
        c = s[1];
        s += 2;
        base = 16;
    }
    if(base == 0)
        base = c == '0' ? 8 : 10;
    /*
        * Compute the cutoff value between legal numbers and illegal
        * numbers.  That is the largest legal value, divided by the
        * base.  An input number that is greater than this value, if
        * followed by a legal input character, is too big.  One that
        * is equal to this value may be valid or not; the limit
        * between valid and invalid numbers is then based on the last
        * digit.  For instance, if the range for longs is
        * [-2147483648..2147483647] and the input base is 10,
        * cutoff will be set to 214748364 and cutlim to either
        * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
        * a value > 214748364, or equal but the next digit is > 7 (or 8),
        * the number is too big, and we will return a range error.
        *
        * Set any if any `digits' consumed; make it negative to indicate
        * overflow.
        */
    cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
    cutlim = cutoff % (unsigned long)base;
    cutoff /= (unsigned long)base;
    for(acc = 0, any = 0;; c = *s++)
    {
        if(isdigit(c))
            c -= '0';
        else if(isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if(c >= base)
            break;
        if(any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else
        {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if(any < 0)
    {
        acc = neg ? LONG_MIN : LONG_MAX;
    }
    else if(neg)
        acc = -acc;
    if(endptr != 0)
        *endptr = (char *) (any ? s - 1 : nptr);
    return (acc);
}

llong_type strtoll(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    ullong_type acc;
    int c;
    ullong_type cutoff;
    int neg = 0, any, cutlim;
    /*
        * Skip white space and pick up leading +/- sign if any.
        * If base is 0, allow 0x for hex and 0 for octal, else
        * assume decimal; if base is already 16, allow 0x.
        */
    do
    {
        c = *s++;
    } while(isspace(c));
    if(c == '-')
    {
        neg = 1;
        c = *s++;
    }
    else if(c == '+')
        c = *s++;
    if((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X'))
    {
        c = s[1];
        s += 2;
        base = 16;
    }
    if(base == 0)
        base = c == '0' ? 8 : 10;
    /*
        * Compute the cutoff value between legal numbers and illegal
        * numbers.  That is the largest legal value, divided by the
        * base.  An input number that is greater than this value, if
        * followed by a legal input character, is too big.  One that
        * is equal to this value may be valid or not; the limit
        * between valid and invalid numbers is then based on the last
        * digit.  For instance, if the range for longs is
        * [-2147483648..2147483647] and the input base is 10,
        * cutoff will be set to 214748364 and cutlim to either
        * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
        * a value > 214748364, or equal but the next digit is > 7 (or 8),
        * the number is too big, and we will return a range error.
        *
        * Set any if any `digits' consumed; make it negative to indicate
        * overflow.
        */
    cutoff = neg ? -(ullong_type)LLONG_MIN : LLONG_MAX;
    cutlim = cutoff % (ullong_type)base;
    cutoff /= (ullong_type)base;
    for(acc = 0, any = 0;; c = *s++)
    {
        if(isdigit(c))
            c -= '0';
        else if(isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if(c >= base)
            break;
        if(any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else
        {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if(any < 0)
    {
        acc = neg ? LLONG_MIN : LLONG_MAX;
    }
    else if(neg)
        acc = -acc;
    if(endptr != 0)
        *endptr = (char *) (any ? s - 1 : nptr);
    return (acc);
}

unsigned long strtoul(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    unsigned long acc;
    int c;
    unsigned long cutoff;
    int neg = 0, any, cutlim;
    /*
        * See strtol for comments as to the logic used.
        */
    do
    {
        c = *s++;
    } while(isspace(c));
    if(c == '-')
    {
        neg = 1;
        c = *s++;
    }
    else if(c == '+')
        c = *s++;
    if((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X'))
    {
        c = s[1];
        s += 2;
        base = 16;
    }
    if(base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
    cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
    for(acc = 0, any = 0;; c = *s++)
    {
        if(isdigit(c))
            c -= '0';
        else if(isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if(c >= base)
            break;
        if(any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else
        {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if(any < 0)
    {
        acc = ULONG_MAX;
    }
    else if(neg)
        acc = -acc;
    if(endptr != 0)
        *endptr = (char *) (any ? s - 1 : nptr);
    return (acc);
}

ullong_type strtoull(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    ullong_type acc;
    int c;
    ullong_type cutoff;
    int neg = 0, any, cutlim;
    /*
        * See strtol for comments as to the logic used.
        */
    do
    {
        c = *s++;
    } while(isspace(c));
    if (c == '-')
    {
        neg = 1;
        c = *s++;
    }
    else if(c == '+')
        c = *s++;
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X'))
    {
        c = s[1];
        s += 2;
        base = 16;
    }
    if(base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = (ullong_type)ULLONG_MAX / (ullong_type)base;
    cutlim = (ullong_type)ULLONG_MAX % (ullong_type)base;
    for(acc = 0, any = 0;; c = *s++)
    {
        if(isdigit(c))
            c -= '0';
        else if(isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if(c >= base)
            break;
        if(any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else
        {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if(any < 0)
    {
        acc = ULLONG_MAX;
    }
    else if(neg)
        acc = -acc;
    if(endptr != 0)
        *endptr = (char *) (any ? s - 1 : nptr);
    return (acc);
}

void abort(void)
{
    while (1)
    {
    }
}
