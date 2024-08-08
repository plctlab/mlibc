/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-17     Bernard      first version
 * 2021-05-02     Meco Man     implement strcpy memmove5
 */

#ifndef MLIBC_STRING_H__
#define MLIBC_STRING_H__

#include "stdint.h"
#include <sys/types.h>

void *memset(void *s, int c, size_t count);
void *memcpy(void *dst, const void *src, size_t count);
int memcmp(const void *cs, const void *ct, size_t count);
void *memmove(void *d, const void *s, size_t n);
void* memchr(const void* m, int c, size_t n);

size_t strlen(const char *s);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, size_t count);
char *strcpy(char *d, const char *s);
char *strncpy(char *dst, const char *src, size_t n);
char *strcat(char * dest, const char * src);
char *strncat(char *dest, const char *src, size_t count);
char* strrchr(const char* s, int c);
char *strchr(const char* str, int c);
char *__strchrnul(const char *s, int c);
char* strstr(const char* string, const char* substring);
char *strrev(char *str);
size_t strcspn(const char* s, const char* c);
char* strtok_r(char* s, const char* delim, char** last);
char* strtok(char* s, const char* delim);
char *strpbrk(const char *s1, const char *s2);
size_t strspn(const char *s, const char *group);
size_t strxfrm(char *dest, const char *src, size_t n);
char *strdup(const char *s);
char *strndup (const char *, size_t);

#endif /*MLIBC_STRING_H__*/
