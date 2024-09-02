/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 
 */

/* 
** Template config format
** You can refer to this configuration format
** of the address space of the link script
*/
__flash =      0x00000000;
__flash_size = 0x00001000;
__ram =        0x20000000;
__ram_size   = 0x00010000;
__stack_size = 1k;

INCLUDE mlibc.ld