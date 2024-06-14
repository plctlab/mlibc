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
#include "uart_pl011.h"

int main() {
    char buf[] = "Hello cortex-a9\n";
    uart_config config = {
        .data_bits = 8,
        .stop_bits = 1,
        .parity = false,
        .baudrate = 9600
    };

    uart_configure(&config);

    printf("%s", buf);

    return 0;
}
