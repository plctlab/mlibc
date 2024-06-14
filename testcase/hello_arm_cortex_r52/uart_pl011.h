/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/6/14   0Bitbiscuits the first version
 */
#ifndef UART_PL011_H
#define UART_PL011_H

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

void UartStdOutInit(void);
unsigned char UartPutc(unsigned char my_ch);

#endif
