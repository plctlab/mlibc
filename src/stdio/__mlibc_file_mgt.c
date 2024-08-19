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

#define FILE_LIST_LOCK_INIT(LOCK) __lock_init(LOCK)
#define FILE_LIST_LOCK_DEINIT(LOCK) __lock_deinit(LOCK)
#define FILE_LIST_LOCK_LOCK(LOCK) __lock_init(LOCK)
#define FILE_LIST_LOCK_UNLOCK(LOCK) __lock_deinit(LOCK)

static FILE *head = NULL;
static _LOCK_T FILE_LIST_LOCK;

FILE *__mlibc_file_add(FILE *f)
{
    f->next = head;
    if(head)
    {
        FILE_LIST_LOCK_INIT(FILE_LIST_LOCK);
        (head)->prev = f;
    }
    head = f;
    return f;
}

void __mlibc_file_remove(FILE *f)
{
    if (f->prev) f->prev->next = f->next;
    if (f->next) f->next->prev = f->prev;
    if (head == f)
    {
        FILE_LIST_LOCK_DEINIT(FILE_LIST_LOCK);
        head = f->next;
    }
}

FILE **__ofl_lock(void)
{
    FILE_LIST_LOCK_LOCK(FILE_LIST_LOCK);
    return &head;
}

void __ofl_unlock(void)
{
    FILE_LIST_LOCK_UNLOCK(FILE_LIST_LOCK);
}