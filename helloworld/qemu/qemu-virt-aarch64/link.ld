/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/29  0Bitbiscuits the first version
 */
OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

__flash =      0x40080000;
__flash_size = 1M;
__ram =        0x50000000;
__ram_size   = 32M;
__stack_size = 1k;

INCLUDE mlibc.ld