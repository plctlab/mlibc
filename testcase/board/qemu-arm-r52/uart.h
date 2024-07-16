/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/14   0Bitbiscuits the first version
 */
#ifndef MLIBC_UART0_H__
#define MLIBC_UART0_H__

#include <compiler.h>

#define PL011_UART0_BASE    0xE7C00000ul
#define __REG32(x)          (*((volatile unsigned int *)(x)))
#define UART_DR(base)   __REG32(base + 0x00)
#define UART_FR(base)   __REG32(base + 0x04)
#define UART_CR(base)   __REG32(base + 0x08)
#define UART_IMSC(base) __REG32(base + 0x0C)
#define UART_ICR(base)  __REG32(base + 0x0C)

mlibc_inline void UartStdOutInit(void){
    /* enable Rx and Tx of UART */
    UART_CR(PL011_UART0_BASE) = (1 << 0);
}

mlibc_inline unsigned char UartPutc(unsigned char my_ch)
{
    while (UART_FR(PL011_UART0_BASE) & (1 << 5u));
    UART_DR(PL011_UART0_BASE) = my_ch;

    return my_ch;
}

#endif /* MLIBC_UART0_H__ */