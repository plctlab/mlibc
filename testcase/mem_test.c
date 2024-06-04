/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <stdlib.h>
#include <stdio.h>

#include <rtthread.h>
void *sbrk(int incr)
{
    return rt_malloc(incr);
}

/**
 * @brief It can successfully run on the QEMU-A9 platform in the RT-Thread/bsp,
 * but if you want to test it on a bare-metal system or a specific operating system,
 * please implement the corresponding portable functions.
 * 
 * @return int 
 */
int mem_test(void)
{
    int *a = malloc(1000);
    if(a)
    {
        *a = 1000;
        printf("a = %d\n", *a);
    }

    int *b = malloc(100);
    if(b)
    {
        *b = 100;
        printf("b = %d\n", *b);
    }
    
    b = realloc(b, 1000);
    if(b)
    {
        *b = 1000;
        printf("b = %d\n", *b);
    }

    int *c = malloc(2048);
    if(c)
    {
        *c = 2048;
        printf("c = %d\n", *c);
    }

    free(a);
    free(b);
    free(c);
    
    return 0;
}
