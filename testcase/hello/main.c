/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/5   0Bitbiscuits the first version
 */
#include <stdio.h>
#include <compiler.h>
#include <uart.h>

ssize_t write(int fd, const void *buf, size_t count)
{
    int i = 0;
    char *buffer = (char*)buf;
    buffer[count] = '\0';
    
    while(*buffer)
    {
        UartPutc(*buffer);
        ++buffer;
    }

    return buffer - (char*)buf;
}

int main(void)
{ 
    UartStdOutInit();

    printf("Hello mlibc\n");
    while(1)
    {
        
    }

    return 0;
}