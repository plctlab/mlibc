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

#define PL011_UART0_BASE    0x10009000u
#define __REG32(x)          (*((volatile unsigned int *)(x)))
#define UART_DR(base)   __REG32(base + 0x00)
#define UART_FR(base)   __REG32(base + 0x18)
#define UART_CR(base)   __REG32(base + 0x30)
#define UART_IMSC(base) __REG32(base + 0x38)
#define UART_ICR(base)  __REG32(base + 0x44)

#endif /* MLIBC_UART0_H__ */