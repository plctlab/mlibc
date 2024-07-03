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

#define PL011_UART0_BASE    0x09000000
#define __REG32(x)          (*((volatile unsigned int *)(x)))
#define UART_DR(base)   __REG32(base + 0x00)
#define UART_FR(base)   __REG32(base + 0x18)
#define UART_CR(base)   __REG32(base + 0x30)
#define UART_IMSC(base) __REG32(base + 0x38)
#define UART_ICR(base)  __REG32(base + 0x44)

const char buf[] = "Hello aarch64\n";

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
    const char *c = buf;
    
    UartStdOutInit();

    printf("Hello mlibc\n");
    while(*c)
    {
        UartPutc(*c);
        ++c;
    }

    while(1)
    {

    }

    return 0;
}