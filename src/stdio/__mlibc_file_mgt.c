/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/23   0Bitbiscuits  the first version
 */
#include "../internal/stdio_impl.h"

FILE **head;
FILE *mlibc_file_add(FILE *f)
{
    f->next = *head;
    if(*head) (*head)->prev = f;
    *head = f;
    return f;
}