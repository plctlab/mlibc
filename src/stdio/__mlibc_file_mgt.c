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

FILE *head = NULL;
FILE *__mlibc_file_add(FILE *f)
{
    f->next = head;
    if(head) (head)->prev = f;
    head = f;
    return f;
}

void __mlibc_file_remove(FILE *f)
{
    if (f->prev) f->prev->next = f->next;
    if (f->next) f->next->prev = f->prev;
    if (head == f) head = f->next;
}

FILE *__mlibc_file_head(void)
{
    return head;
}
