/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/4   0Bitbiscuits the first version
 */
#include <stdio.h>

/**
 * @brief It can successfully run on the QEMU-A9 platform in the RT-Thread/bsp,
 * but if you want to test it on a bare-metal system or a specific operating system,
 * please implement the corresponding portable functions.
 * 
 * @return int 
 */
int hello_mlibc(void)
{
    printf("Hello mlibc\n");

    return 0;
}
