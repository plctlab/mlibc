/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/25  0Bitbiscuits the first version
 */
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <compiler.h>
#include "../internal/time_impl.h"

#define STRFTIME_BUFFER_SIZE 64

/* days per month -- nonleap! */
static const short __spm[13] =
{
    0,
    (31),
    (31 + 28),
    (31 + 28 + 31),
    (31 + 28 + 31 + 30),
    (31 + 28 + 31 + 30 + 31),
    (31 + 28 + 31 + 30 + 31 + 30),
    (31 + 28 + 31 + 30 + 31 + 30 + 31),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30),
    (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31),
};

static flag_handler get_flag_handler(char flag)
{
    char tmp[100];
    FILE f;
    const flag_handler_pair_t *handler = islower(flag) ? \
        handler_table.lower_handlers : handler_table.upper_handlers;
    
    while(handler && handler->handler)
    {
        if(handler->flag == flag)
            return handler->handler;
        handler++;
    }

    return NULL;
}

#ifndef __isleap
mlibc_inline int __isleap(int year)
{
    /* every fourth year is a leap year except for century years that are
     * not divisible by 400. */
    /*  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)); */
    return (!(year % 4) && ((year % 100) || !(year % 400)));
}
#endif

mlibc_weak struct tm *gmtime_r(const time_t *timep, struct tm *r)
{
    int i;
    int work;

    if(timep == NULL || r == NULL)
    {
        return NULL;
    }
    
    memset(r, 0, sizeof(struct tm));            // init struct tm

    work = (*timep) % (24 * 60 * 60);           // seconds pass today
    r->tm_sec = work % 60;
    work /= 60;
    r->tm_min = work % 60;
    r->tm_hour = work / 60;

    work = (int)(*timep / (24 * 60 * 60));      // days starting from 1970
    r->tm_wday = (4 + work) % 7;                // weeks starting from 1970(1970.1.1 is Thursday)

    for (i = 1970;; ++i)                        // calculate how many days have passed this year
    {
        int k = __isleap(i) ? 366 : 365;
        if (work >= k)
            work -= k;
        else
            break;
    }
    r->tm_year = i - 1900;                      // 1900 is the base time for computers
    r->tm_yday = work;                          // days have passed this year

    r->tm_mday = 1;                             
    if (__isleap(i) && (work > 58))
    {
        if (work == 59)
            r->tm_mday = 2; /* 29.2. */
        work -= 1;
    }

    for (i = 11; i && (__spm[i] > work); --i);  
    r->tm_mon = i;
    r->tm_mday += work - __spm[i];              // calculate how many days have passed this month

    r->tm_isdst = 0;                            // not support dst

    return r;
}

mlibc_weak struct tm* gmtime(const time_t* t)
{
    static struct tm tmp;
    return gmtime_r(t, &tmp);
}

mlibc_weak struct tm* localtime_r(const time_t* t, struct tm* r)
{
    return gmtime_r(t, r);
}

/**
 * @brief not support local_t
 * 
 * @param s 
 * @param buf_size 
 * @param format 
 * @param tp 
 * @return size_t 
 */
size_t strftime(char *s, size_t buf_size, const char *format, const struct tm *tp)
{
    char buffer[STRFTIME_BUFFER_SIZE];
    const char *format_ptr = format;
    char *s_ptr = s;
    size_t length = 0;
    size_t result = 0;
    flag_handler entry = NULL;

    while (*format_ptr && length < buf_size - 1) {
        if (*format_ptr == '%') {
            format_ptr++;
            entry = get_flag_handler(*format_ptr);

            result = entry == NULL ? sprintf(buffer, "%%c", *format_ptr) \
                    : entry(buffer, sizeof(buffer), tp);
            if (result + length > buf_size) {
                return 0;
            }

            strncpy(s_ptr, buffer, result);
            s_ptr += result;
            length += result;
            format_ptr++;
        } else {
            *s_ptr++ = *format_ptr++;
            length++;
        }
    }

    *s_ptr = '\0';
    return length;
}
