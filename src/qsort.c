/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/5/5       0Bitbiscuits    realize qsort
 */
#include <stdlib.h>
#include <stdint.h>

typedef int (*__MLIBC_cmp_t)(const void *, const void *);

static void __MLIBC_swap(uint8_t *a, uint8_t *b, size_t size)
{
    uint8_t tmp = 0;
    size_t i = 0;

    for(; i < size; ++i)
    {
        tmp = *(a + i);
        *(a + i) = *(b + i);
        *(b + i) = tmp;
    }
}

static void __MLIBC_insertion_sort(uint8_t *arr, size_t n, size_t size, __MLIBC_cmp_t cmp)
{
    size_t i = 1;
    size_t j = 0;

    for(; i < n; ++i)
    {
        // compare with the previous one
        for(j = i; j > 0 && cmp(arr + j * size, arr + (j - 1) * size) < 0; --j)
        {
            __MLIBC_swap(arr + j * size, arr + (j - 1) * size, size);
        }
    }
}

static uint8_t *__MLIBC_get_median(uint8_t *beg, uint8_t *mid, uint8_t *end, __MLIBC_cmp_t cmp) 
{
    /* find mid elem */
    return cmp(beg, mid) < 0 ? 
            (cmp(mid, end)) < 0 ? mid : (cmp(beg, end) < 0 ? end : beg) :
            (cmp(beg, end)) < 0 ? beg : (cmp(mid, end) < 0 ? end : mid);
}

void __MLIBC_qsort_func(uint8_t *pbeg, uint8_t *pend, size_t elem_size, __MLIBC_cmp_t cmp)
{
    size_t nelem = pend > pbeg ? (pend - pbeg) / elem_size + 1 : 0;
    uint8_t *pmid = NULL;
    uint8_t *pleft = NULL;
    uint8_t *pright = NULL;

    if (nelem < 7) 
    {
        if(nelem > 0)
        {
            __MLIBC_insertion_sort(pbeg, nelem, elem_size, cmp);
        }
        return;
    }
    
    pmid = pbeg + (nelem >> 1) * elem_size;

    // find mid and choose mid as base
    pmid = __MLIBC_get_median(pbeg, pmid, pend, cmp);
    __MLIBC_swap(pbeg, pmid, elem_size);

    pleft = pbeg + elem_size;
    pright = pend;
    while(pleft < pright)
    {
        while(pleft < pright && cmp(pright, pbeg) >= 0)
        { 
            pright -= elem_size;
        }
        while(pleft < pright && cmp(pleft, pbeg) <= 0)
        {
            pleft += elem_size;
        }
        __MLIBC_swap(pleft, pright, elem_size);
    }

    __MLIBC_swap(pbeg, pleft, elem_size);

    __MLIBC_qsort_func(pbeg, pleft - elem_size, elem_size, cmp);
    __MLIBC_qsort_func(pleft + elem_size, pend, elem_size, cmp);
}

void qsort (void *arr, size_t nelem, size_t elem_size, int (*cmp)(const void *, const void *))
{
    uint8_t *pbeg = NULL;
    uint8_t *pend = NULL;

    if(arr == NULL || nelem == 0 || elem_size == 0 || cmp == NULL)
        return;
    
    // get the begin, end of array
    pbeg = arr;
    pend = pbeg + (nelem - 1) * elem_size;

    __MLIBC_qsort_func(pbeg, pend, elem_size, cmp);
}