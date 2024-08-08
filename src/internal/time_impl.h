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
#include <stdio.h>
#include <sys/types.h>
#include <compiler.h>
#include <ctype.h>
#include <stddef.h>

typedef int (* flag_handler)(char *, size_t, const struct tm *);

typedef struct
{
    char flag;
    flag_handler handler;
} flag_handler_pair_t;

typedef struct
{
    const flag_handler_pair_t* upper_handlers;
    const flag_handler_pair_t* lower_handlers;
} flag_handler_table_t;

mlibc_inline int flag_Y_handler(char *buffer, size_t buf_size, const struct tm *tm)
{
    return snprintf(buffer, buf_size, "%04d", 1900 + tm->tm_year);
}

mlibc_inline int flag_m_handler(char *buffer, size_t buf_size, const struct tm *tm)
{
    return snprintf(buffer, sizeof(buffer), "%02d", tm->tm_mon + 1);
}

mlibc_inline int flag_d_handler(char *buffer, size_t buf_size, const struct tm *tm)
{
    return snprintf(buffer, sizeof(buffer), "%02d", tm->tm_mday);
}

mlibc_inline int flag_H_handler(char *buffer, size_t buf_size, const struct tm *tm)
{
    return snprintf(buffer, sizeof(buffer), "%02d", tm->tm_hour);
}

mlibc_inline int flag_M_handler(char *buffer, size_t buf_size, const struct tm *tm)
{
    return snprintf(buffer, sizeof(buffer), "%02d", tm->tm_min);
}

mlibc_inline int flag_S_handler(char *buffer, size_t buf_size, const struct tm *tm)
{
    return snprintf(buffer, sizeof(buffer), "%02d", tm->tm_sec);
}

mlibc_inline int flag_percentage_handler(char *buffer, size_t buf_size, const struct tm *tm)
{
    return snprintf(buffer, sizeof(buffer), "%%");
}

static const flag_handler_pair_t upper_handlers[] = 
{
    {'Y', flag_Y_handler},
    {'H', flag_H_handler},
    {'M', flag_M_handler},
    {'S', flag_S_handler},
    {'%', flag_percentage_handler},
    {'\0', NULL}
};

static const flag_handler_pair_t lower_handlers[] = 
{
    {'m', flag_m_handler},
    {'d', flag_d_handler},
    {'\0', NULL}
};

static const flag_handler_table_t handler_table = 
{
    .upper_handlers = upper_handlers,
    .lower_handlers = lower_handlers
};
