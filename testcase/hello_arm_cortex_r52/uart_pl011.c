/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/14   0Bitbiscuits the first version
 */
#include <sys/types.h>
#include "uart_pl011.h"

void UartStdOutInit(void)
{
    CMSDK_UART0->BAUDDIV = 16;
    CMSDK_UART0->CTRL    = 0x41; // High speed test mode, TX only
    return;
}

// Output a character
unsigned char UartPutc(unsigned char my_ch)
{
    while ((CMSDK_UART0->STATE & 1)); // Wait if Transmit Holding register is full
    CMSDK_UART0->DATA = my_ch; // write to transmit holding register
    return (my_ch);
}

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