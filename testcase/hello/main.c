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

static void UartStdOutInit(void){
    /* enable Rx and Tx of UART */
    UART_CR(PL011_UART0_BASE) = (1 << 0);
}

static unsigned char UartPutc(unsigned char my_ch)
{
    while (UART_FR(PL011_UART0_BASE) & (1 << 5u));
    UART_DR(PL011_UART0_BASE) = my_ch;

    return my_ch;
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

int main(void)
{ 
    UartStdOutInit();

    printf("Hello mlibc\n");
    while(1)
    {
        
    }

    return 0;
}