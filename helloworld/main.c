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
#include <stdlib.h>

ssize_t write(int fd, const void *buf, size_t count)
{
    int i = 0;
    char *buffer = (char*)buf;
    
    while(count--)
    {
        UartPutc(*buffer);
        ++buffer;
    }

    return buffer - (char*)buf;
}

int main(int argc, char **argv)
{
    char buf[] = "Uart Test\n";
    char *c = buf;
    
    UartStdOutInit();
    while(c && *c != '\0')
    {
        UartPutc(*c);
        c++;
    }

    printf("Hello mlibc\n");
    while(1)
    {
        
    }

    return 0;
}