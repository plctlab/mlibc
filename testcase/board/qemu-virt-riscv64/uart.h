/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/5   0Bitbiscuits the first version
 */
#ifndef MLIBC_UART0_H__
#define MLIBC_UART0_H__

#include <stdint.h>
#include <compiler.h>

#define UART_RHR 0
#define UART_THR 0

/* INTERRUPT ENABLE REGISTER */
#define UART_IER 1
#define UART_IER_RX_ENABLE (1 << 0)
#define UART_IER_TX_ENABLE (1 << 1)

/* FIFO CONTROL REGISTER */
#define UART_FCR 2
#define UART_FCR_FIFO_ENABLE (1 << 0)
#define UART_FCR_FIFO_CLEAR (3 << 1)

/* INTERRUPT STATUS REGISTER */
#define UART_ISR 2

/* LINE CONTROL REGISTER */
#define UART_LCR 3
#define UART_LCR_EIGHT_BITS (3 << 0)
// special mode to set baud rate
#define UART_LCR_BAUD_LATCH (1 << 7)

/* LINE STATUS REGISTER */
#define UART_LSR 5
// input is waiting to be read from RHR
#define UART_LSR_RX_READY (1 << 0)
// THR can accept another character to send
#define UART_LSR_TX_IDLE (1 << 5)

#define UART_REFERENCE_CLOCK  1843200
#define UART_DEFAULT_BAUDRATE 115200

void *uart0_base = (void*)0x10000000;

static inline void __raw_writeb(uint8_t val, volatile void *addr)
{
    asm volatile("sb %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline uint8_t __raw_readb(const volatile void *addr)
{
    uint8_t val;

    asm volatile("lb %0, 0(%1)" : "=r"(val) : "r"(addr));
    return val;
}

#define write8_uart0(idx, value) __raw_writeb(((uint8_t)value), (void*)((size_t)uart0_base + (idx)))
#define read8_uart0(idx) __raw_readb((void*)((size_t)uart0_base + (idx)))

mlibc_inline void UartStdOutInit(void)
{
    uint32_t div = UART_REFERENCE_CLOCK / (UART_DEFAULT_BAUDRATE * 16);

    write8_uart0(UART_IER, 0x00);
    write8_uart0(UART_LCR, UART_LCR_BAUD_LATCH);

    // LSB
    write8_uart0(0, div & 0xff);
    // MSB
    write8_uart0(1, (div >> 8) & 0xff);

    // set word length to 8 bits, no parity
    write8_uart0(UART_LCR, UART_LCR_EIGHT_BITS);

    write8_uart0(UART_FCR, UART_FCR_FIFO_ENABLE | UART_FCR_FIFO_CLEAR);

    return;
}

mlibc_inline unsigned char UartPutc(unsigned char my_ch)
{
    // wait for Transmit Holding Empty to be set in LSR.
    while((read8_uart0(UART_LSR) & UART_LSR_TX_IDLE) == 0);
    write8_uart0(UART_THR, my_ch);

    return my_ch;
}

#endif /* MLIBC_UART0_H__ */