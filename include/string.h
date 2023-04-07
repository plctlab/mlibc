/*
 * Copyright (c) 2023
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/2/1       linshire     the first version
 */
 
#ifndef __STRING_H_
#define __STRING_H_

#include <typedef.h>

size_t strlen(const char *s);
void *memset (void *dstpp, int c, size_t len);
void *memcpy(void *dst, const void *src, size_t n);
void *memccpy(void *dst, const void *src, int c, size_t n);

#endif
