/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first version
 */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
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

