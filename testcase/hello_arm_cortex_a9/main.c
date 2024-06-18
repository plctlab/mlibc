/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/14   0Bitbiscuits the first version
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define FR_BUSY         (1 << 3u)
#define CR_UARTEN       (1 << 0u)
#define LCRH_WLEN_8BITS (3u << 5u)
#define LCRH_FEN        (1 << 4u)
#define LCRH_PEN        (1 << 1u)
#define LCRH_STP2       (1 << 3u)
#define LCRH_EPS        (1 << 2u)
#define LCRH_SPS        (1 << 7u)
#define FR_TXFF         (1 << 5u)

typedef volatile struct __attribute__((packed)) {
        uint32_t DR;                            /* 0x0 Data Register */
        uint32_t RSRECR;                        /* 0x4 Receive status / error clear register */
        uint32_t _reserved0[4];                 /* 0x8 - 0x14 reserved */
        const uint32_t FR;                      /* 0x18 Flag register */
        uint32_t _reserved1;                    /* 0x1C reserved */
        uint32_t ILPR;                          /* 0x20 Low-power counter register */
        uint32_t IBRD;                          /* 0x24 Integer baudrate register */
        uint32_t FBRD;                          /* 0x28 Fractional baudrate register */
        uint32_t LCRH;                          /* 0x2C Line control register */
        uint32_t CR;                            /* 0x30 Control register */
} uart_registers;

static uart_registers* uart0 = (uart_registers*)0x10009000u;

void UartStdOutInit(void){
    /* Disable the UART */
    uart0->CR &= ~CR_UARTEN;
    /* Finish any current transmission, and flush the FIFO */
    while (uart0->FR & FR_BUSY);
    uart0->LCRH &= ~LCRH_FEN;

    /* Set baudrate */
    double intpart, fractpart;
    double baudrate_divisor = (double)24000000u / (16u * 9600);
    
    intpart = (int)baudrate_divisor;
    fractpart = baudrate_divisor - intpart;

    uart0->IBRD = (uint16_t)intpart;
    uart0->FBRD = (uint8_t)((fractpart * 64u) + 0.5);

    uint32_t lcrh = 0u;

    lcrh |= LCRH_WLEN_8BITS;

    lcrh &= ~LCRH_PEN;
    lcrh &= ~LCRH_EPS;
    lcrh &= ~LCRH_SPS;
    lcrh &= ~LCRH_STP2;

    /* Enable FIFOs */
    lcrh |= LCRH_FEN;

    uart0->LCRH = lcrh;

    /* Enable the UART */
    uart0->CR |= CR_UARTEN;
}

unsigned char UartPutc(unsigned char my_ch)
{
    while (uart0->FR & FR_TXFF);
    uart0->DR = my_ch;
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

int main() {
    char buf[] = "Hello cortex-a9\n";
    char *c = buf;
    FILE f;

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

