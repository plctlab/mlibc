/*
 * Copyright (c) 2023
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023/2/1       linshire     the first version
 * 2023/3/11      linshire     add memset
 */

#include <string.h>
#include <typedef.h>

size_t strlen(const char *s)
{
    const char *a = s;
    for (; *s; s++);
    return s-a;
}

void *memset (void *dstpp, int c, size_t len)
{
    long int dstp = (long int) dstpp;
    
    if (len >= 8)
    {
        size_t xlen;
        size_t cccc;
        
        cccc = (unsigned char) c;
        cccc |= cccc << 8;
        cccc |= cccc << 16;
        if (opt_size > 4)
        /* Do the shift in two steps to avoid warning if long has 32 bits.  */
            cccc |= (cccc << 16) << 16;
        
        /* There are at least some bytes to set.
         No need to test for LEN == 0 in this alignment loop.  */
        while (dstp % opt_size != 0)
        {
            ((uint8_t *) dstp)[0] = c;
            dstp += 1;
            len -= 1;
        }
        
        /* Write 8 `size_t' per iteration until less than 8 `size_t' remain.  */
        xlen = len / (opt_size * 8);
        while (xlen > 0)
        {
            ((size_t *) dstp)[0] = cccc;
            ((size_t *) dstp)[1] = cccc;
            ((size_t *) dstp)[2] = cccc;
            ((size_t *) dstp)[3] = cccc;
            ((size_t *) dstp)[4] = cccc;
            ((size_t *) dstp)[5] = cccc;
            ((size_t *) dstp)[6] = cccc;
            ((size_t *) dstp)[7] = cccc;
            dstp += 8 * opt_size;
            xlen -= 1;
        }
        len %= opt_size * 8;
        
        /* Write 1 `size_t' per iteration until less than opt_size bytes remain.  */
        xlen = len / opt_size;
        while (xlen > 0)
        {
            ((size_t *) dstp)[0] = cccc;
            dstp += opt_size;
            xlen -= 1;
        }
        len %= opt_size;
    }
    
    /* Write the last few bytes.  */
    while (len > 0)
    {
        ((uint8_t *) dstp)[0] = c;
        dstp += 1;
        len -= 1;
    }
    
    return dstpp;
}


void *memcpy(void *dst, const void *src, size_t n)
{
    #define UNALIGNED(X, Y) \
    (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))
    #define BIGBLOCKSIZE    (sizeof (long) << 2)
    #define LITTLEBLOCKSIZE (sizeof (long))
    #define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

    char *dst_ptr = (char *)dst;
    char *src_ptr = (char *)src;
    long *aligned_dst = NULL;
    long *aligned_src = NULL;
    size_t len = n;

    /* If the size is small, or either SRC or DST is unaligned,
    then punt into the byte copy loop.  This should be rare. */
    if (!TOO_SMALL(len) && !UNALIGNED(src_ptr, dst_ptr))
    {
        aligned_dst = (long *)dst_ptr;
        aligned_src = (long *)src_ptr;

        /* Copy 4X long words at a time if possible. */
        while (len >= BIGBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            len -= BIGBLOCKSIZE;
        }

        /* Copy one long word at a time if possible. */
        while (len >= LITTLEBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            len -= LITTLEBLOCKSIZE;
        }

        /* Pick up any residual with a byte copier. */
        dst_ptr = (char *)aligned_dst;
        src_ptr = (char *)aligned_src;
    }

    while (len--)
        *dst_ptr++ = *src_ptr++;

    return dst;
#undef UNALIGNED
#undef BIGBLOCKSIZE
#undef LITTLEBLOCKSIZE
#undef TOO_SMALL

}

void *memccpy(void *dst, const void *src, int c, size_t n)
{
    char *dst_ptr = (char *)dst;
    char *src_ptr = (char *)src;
    size_t len = n;
    size_t num = 0;
    int chr = c;

    for(num = 0; (num < n) && (chr != *(src_ptr + num)); num ++  );

    if(!num || (chr != *(src_ptr + num)))

    return NULL;

    return memcpy(dst_ptr, src, num +1);

}
