/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/06/16     bernard      the first verison
 */

#ifndef MLIBC_CTYPE_H__
#define MLIBC_CTYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

static inline int isupper(int a)
{
    return (int)(((unsigned)(a)-(unsigned)'A') < 26U);
}

static inline int isalpha(int c)
{
    return (int)((((unsigned)c|32u)-(unsigned)'a') < 26U);
}

static inline int isspace(int c)
{
    return (int)(c == (int)' ' || ((unsigned)c-(unsigned)'\t') < 5U);
}

static inline int isgraph(int c)
{
    return (int)((((unsigned)c) > ' ') && (((unsigned)c) <= (unsigned)'~'));
}

static inline int isprint(int c)
{
    return (int)((((unsigned)c) >= ' ') && (((unsigned)c) <= (unsigned)'~'));
}

static inline int isdigit(int a)
{
    return (int)(((unsigned)(a)-(unsigned)'0') < 10U);
}

static inline int isxdigit(int a)
{
    unsigned int ua = (unsigned int)a;

    return (int)(((ua - (unsigned)'0') < 10U) || ((ua | 32U) - (unsigned)'a' < 6U));
}

static inline int tolower(int chr)
{
    return (chr >= (int)'A' && chr <= (int)'Z') ? (chr + 32) : (chr);
}

static inline int islower(int c)
{
    return ((unsigned)c - 'a') < 26;
}

static inline int toupper(int chr)
{
    return (int)((chr >= (int)'a' && chr <= (int)'z') ? (chr - 32) : (chr));
}

static inline int isalnum(int chr)
{
    return (int)(isalpha(chr) || isdigit(chr));
}

static inline int isblank(int c)
{
    return (c == ' ' || c == '\t');
}

static inline int iscntrl(int c)
{
    return ((unsigned)c < 0x20 || c == 0x7F);
}

static inline int ispunct(int c)
{
    return isgraph(c) && !isalnum(c);
}

#ifdef __cplusplus
}
#endif

#endif /*MLIBC_CTYPE_H__*/
