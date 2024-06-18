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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define   __I     volatile const       /*!< Defines 'read only' permissions */
#define   __O     volatile             /*!< Defines 'write only' permissions */
#define   __IO    volatile             /*!< Defines 'read / write' permissions */

typedef struct
{
    __IO   unsigned  DATA;                     /* Offset: 0x000 (R/W) Data Register    */
    __IO   unsigned  STATE;                    /* Offset: 0x004 (R/W) Status Register  */
    __IO   unsigned  CTRL;                     /* Offset: 0x008 (R/W) Control Register */
    union {
      __I    unsigned  INTSTATUS;              /* Offset: 0x00C (R/ ) Interrupt Status Register */
      __O    unsigned  INTCLEAR;               /* Offset: 0x00C ( /W) Interrupt Clear Register  */
      };
    __IO   unsigned  BAUDDIV;                  /* Offset: 0x010 (R/W) Baudrate Divider Register */
} CMSDK_UART_TypeDef;

#define CMSDK_UART0_BASE         (0xE7C00000ul)       /* UART 0 1 Base Address */
#define CMSDK_UART0             ((CMSDK_UART_TypeDef   *) CMSDK_UART0_BASE   )

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

int main(void)
{   
    char buf[] = "Hello cortex-r52\n";
    char *c = buf;

    UartStdOutInit();
    
    printf("Hello mlibc\n");
    while(*c)
    {
        UartPutc(*c);
        ++c;
    }
    
    // 用户代码
    while (1)
    {
        // 主循环
    }
    return 0;
}


