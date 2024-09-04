/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/23  0Bitbiscuits the first version
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define argc 0
#define argv NULL

extern char __data_source[];
extern char __data_start[];
extern char __data_end[];
extern char __data_size[];
extern char __bss_start[];
extern char __bss_end[];
extern char __bss_size[];

void _start(void);
int main(int, char **);

/* Copy the data segment and the bss segment and call the main function. */
static inline void __start(void)
{
    memcpy(__data_start, __data_source, (uintptr_t) __data_size);
    memset(__bss_start, '\0', (uintptr_t) __bss_size);
    int ret = main(argc, argv);
    exit(ret);
}