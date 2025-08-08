/*
 * test_memory_ops.c - PSE51 string.h memory operation test suite
 * Tests: memchr, memcmp, memcpy, memmove, memset
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for memory operation functions
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>

#include "../test_colors.h"
#define TEST_SIZE 1024
#define LARGE_SIZE (1024 * 1024)

/* Test memset() function */
void test_memset(void) {
    unsigned char buffer[TEST_SIZE];
    size_t i;
    void *result;
    
    TEST_START("memset()");
    
    /* Test basic memset */
    result = memset(buffer, 0xAA, TEST_SIZE);
    assert(result == buffer);
    
    for (i = 0; i < TEST_SIZE; i++) {
        assert(buffer[i] == 0xAA);
    }
    
    /* Test zero fill */
    memset(buffer, 0, TEST_SIZE);
    for (i = 0; i < TEST_SIZE; i++) {
        assert(buffer[i] == 0);
    }
    
    /* Test partial memset */
    memset(buffer, 0xFF, TEST_SIZE);
    memset(buffer + 100, 0x55, 50);
    
    for (i = 0; i < 100; i++) {
        assert(buffer[i] == 0xFF);
    }
    for (i = 100; i < 150; i++) {
        assert(buffer[i] == 0x55);
    }
    for (i = 150; i < TEST_SIZE; i++) {
        assert(buffer[i] == 0xFF);
    }
    
    /* Test zero-length memset */
    unsigned char save = buffer[0];
    memset(buffer, 0x99, 0);
    assert(buffer[0] == save); /* Should not change */
    
    /* Test alignment */
    for (i = 0; i < 8; i++) {
        memset(buffer + i, 0x77, 64);
        size_t j;
        for (j = i; j < i + 64; j++) {
            assert(buffer[j] == 0x77);
        }
    }
    
    /* Test with different data types */
    int int_buffer[256];
    memset(int_buffer, 0, sizeof(int_buffer));
    for (i = 0; i < 256; i++) {
        assert(int_buffer[i] == 0);
    }
    
    /* Test pattern fill */
    struct {
        char a;
        int b;
        char c;
    } struct_buffer[10];
    
    memset(struct_buffer, 0xFF, sizeof(struct_buffer));
    unsigned char *p = (unsigned char *)struct_buffer;
    for (i = 0; i < sizeof(struct_buffer); i++) {
        assert(p[i] == 0xFF);
    }
    
    TEST_PASSED("memset()");
}

/* Test memcpy() function */
void test_memcpy(void) {
    unsigned char src[TEST_SIZE];
    unsigned char dst[TEST_SIZE];
    size_t i;
    void *result;
    
    TEST_START("memcpy()");
    
    /* Initialize source */
    for (i = 0; i < TEST_SIZE; i++) {
        src[i] = i & 0xFF;
    }
    
    /* Test basic copy */
    memset(dst, 0, TEST_SIZE);
    result = memcpy(dst, src, TEST_SIZE);
    assert(result == dst);
    
    for (i = 0; i < TEST_SIZE; i++) {
        assert(dst[i] == src[i]);
    }
    
    /* Test partial copy */
    memset(dst, 0xFF, TEST_SIZE);
    memcpy(dst + 100, src + 200, 300);
    
    for (i = 0; i < 100; i++) {
        assert(dst[i] == 0xFF);
    }
    for (i = 100; i < 400; i++) {
        assert(dst[i] == src[i + 100]);
    }
    for (i = 400; i < TEST_SIZE; i++) {
        assert(dst[i] == 0xFF);
    }
    
    /* Test zero-length copy */
    unsigned char save = dst[0];
    memcpy(dst, src, 0);
    assert(dst[0] == save); /* Should not change */
    
    /* Test unaligned copies */
    for (i = 1; i < 8; i++) {
        memset(dst, 0, TEST_SIZE);
        memcpy(dst + i, src + i, 100);
        size_t j;
        for (j = 0; j < i; j++) {
            assert(dst[j] == 0);
        }
        for (j = i; j < i + 100; j++) {
            assert(dst[j] == src[j]);
        }
    }
    
    /* Test copying structures */
    struct test_struct {
        int a;
        char b[20];
        double c;
    } s1 = {42, "Hello, World!", 3.14}, s2;
    
    memcpy(&s2, &s1, sizeof(struct test_struct));
    assert(s2.a == s1.a);
    assert(strcmp(s2.b, s1.b) == 0);
    assert(s2.c == s1.c);
    
    /* Test large copy */
    unsigned char *large_src = (unsigned char *)malloc(LARGE_SIZE);
    unsigned char *large_dst = (unsigned char *)malloc(LARGE_SIZE);
    
    if (large_src && large_dst) {
        for (i = 0; i < LARGE_SIZE; i++) {
            large_src[i] = (i * 7) & 0xFF;
        }
        
        memcpy(large_dst, large_src, LARGE_SIZE);
        
        /* Verify samples */
        assert(large_dst[0] == large_src[0]);
        assert(large_dst[LARGE_SIZE/2] == large_src[LARGE_SIZE/2]);
        assert(large_dst[LARGE_SIZE-1] == large_src[LARGE_SIZE-1]);
        
        free(large_src);
        free(large_dst);
    }
    
    TEST_PASSED("memcpy()");
}

/* Test memmove() function */
void test_memmove(void) {
    unsigned char buffer[TEST_SIZE];
    size_t i;
    void *result;
    
    TEST_START("memmove()");
    
    /* Initialize buffer */
    for (i = 0; i < TEST_SIZE; i++) {
        buffer[i] = i & 0xFF;
    }
    
    /* Test non-overlapping move (like memcpy) */
    unsigned char dst[TEST_SIZE];
    result = memmove(dst, buffer, TEST_SIZE);
    assert(result == dst);
    
    for (i = 0; i < TEST_SIZE; i++) {
        assert(dst[i] == buffer[i]);
    }
    
    /* Test overlapping move forward */
    for (i = 0; i < 100; i++) {
        buffer[i] = i;
    }
    memmove(buffer + 50, buffer, 50);
    
    for (i = 0; i < 50; i++) {
        assert(buffer[i] == i);
    }
    for (i = 50; i < 100; i++) {
        assert(buffer[i] == (i - 50));
    }
    
    /* Test overlapping move backward */
    for (i = 0; i < 100; i++) {
        buffer[i] = i;
    }
    memmove(buffer, buffer + 25, 50);
    
    for (i = 0; i < 50; i++) {
        assert(buffer[i] == (i + 25));
    }
    
    /* Test complete overlap (src == dst) */
    unsigned char save[100];
    memcpy(save, buffer, 100);
    memmove(buffer, buffer, 100);
    
    for (i = 0; i < 100; i++) {
        assert(buffer[i] == save[i]);
    }
    
    /* Test zero-length move */
    save[0] = buffer[0];
    memmove(buffer, buffer + 10, 0);
    assert(buffer[0] == save[0]);
    
    /* Test various overlap scenarios */
    /* Small overlap at end */
    for (i = 0; i < 20; i++) {
        buffer[i] = i;
    }
    memmove(buffer + 10, buffer + 5, 10);
    for (i = 0; i < 10; i++) {
        assert(buffer[i] == i);
    }
    for (i = 10; i < 20; i++) {
        assert(buffer[i] == (i - 5));
    }
    
    /* Test with structures */
    struct {
        int nums[10];
    } s1, s2;
    
    for (i = 0; i < 10; i++) {
        s1.nums[i] = i * i;
    }
    
    memmove(&s2, &s1, sizeof(s1));
    for (i = 0; i < 10; i++) {
        assert(s2.nums[i] == s1.nums[i]);
    }
    
    /* Test overlapping array elements */
    int arr[20];
    for (i = 0; i < 20; i++) {
        arr[i] = i;
    }
    
    memmove(&arr[5], &arr[0], 10 * sizeof(int));
    for (i = 0; i < 5; i++) {
        assert(arr[i] == i);
    }
    for (i = 5; i < 15; i++) {
        assert(arr[i] == (i - 5));
    }
    
    TEST_PASSED("memmove()");
}

/* Test memcmp() function */
void test_memcmp(void) {
    unsigned char buf1[TEST_SIZE];
    unsigned char buf2[TEST_SIZE];
    size_t i;
    int result;
    
    TEST_START("memcmp()");
    
    /* Test equal buffers */
    for (i = 0; i < TEST_SIZE; i++) {
        buf1[i] = buf2[i] = i & 0xFF;
    }
    
    result = memcmp(buf1, buf2, TEST_SIZE);
    assert(result == 0);
    
    /* Test first byte different */
    buf2[0] = buf1[0] + 1;
    result = memcmp(buf1, buf2, TEST_SIZE);
    assert(result < 0);
    
    result = memcmp(buf2, buf1, TEST_SIZE);
    assert(result > 0);
    
    buf2[0] = buf1[0]; /* Restore */
    
    /* Test last byte different */
    buf2[TEST_SIZE - 1] = buf1[TEST_SIZE - 1] + 1;
    result = memcmp(buf1, buf2, TEST_SIZE);
    assert(result < 0);
    
    buf2[TEST_SIZE - 1] = buf1[TEST_SIZE - 1]; /* Restore */
    
    /* Test middle byte different */
    buf2[TEST_SIZE / 2] = buf1[TEST_SIZE / 2] + 1;
    result = memcmp(buf1, buf2, TEST_SIZE);
    assert(result < 0);
    
    /* Test zero length comparison */
    buf1[0] = 0;
    buf2[0] = 255;
    result = memcmp(buf1, buf2, 0);
    assert(result == 0);
    
    /* Test partial comparison */
    for (i = 0; i < TEST_SIZE; i++) {
        buf1[i] = i & 0xFF;
        buf2[i] = i & 0xFF;
    }
    buf2[100] = buf1[100] + 1;
    
    result = memcmp(buf1, buf2, 50);
    assert(result == 0); /* First 50 bytes are equal */
    
    result = memcmp(buf1, buf2, 101);
    assert(result < 0); /* Difference at byte 100 */
    
    /* Test with different signedness interpretation */
    unsigned char ubuf1[] = {0x7F, 0x80, 0xFF};
    unsigned char ubuf2[] = {0x7F, 0x7F, 0xFF};
    
    result = memcmp(ubuf1, ubuf2, 3);
    assert(result > 0); /* 0x80 > 0x7F when compared as unsigned */
    
    /* Test alignment effects */
    for (i = 0; i < 8; i++) {
        memset(buf1, 0xAA, TEST_SIZE);
        memset(buf2, 0xAA, TEST_SIZE);
        
        result = memcmp(buf1 + i, buf2 + i, 64);
        assert(result == 0);
        
        buf2[i + 32] = 0xBB;
        result = memcmp(buf1 + i, buf2 + i, 64);
        assert(result < 0);
    }
    
    /* Test comparing structures */
    struct {
        int a;
        char b;
        /* May have padding here */
        int c;
    } s1 = {1, 'A', 3}, s2 = {1, 'A', 3};
    
    /* Note: This may fail if padding contains different values */
    /* Better to compare individual fields in practice */
    
    /* Test self comparison */
    result = memcmp(buf1, buf1, TEST_SIZE);
    assert(result == 0);
    
    TEST_PASSED("memcmp()");
}

/* Test memchr() function */
void test_memchr(void) {
    unsigned char buffer[TEST_SIZE];
    size_t i;
    void *result;
    
    TEST_START("memchr()");
    
    /* Initialize buffer */
    for (i = 0; i < TEST_SIZE; i++) {
        buffer[i] = i & 0xFF;
    }
    
    /* Test finding existing byte */
    result = memchr(buffer, 0x42, TEST_SIZE);
    assert(result != NULL);
    assert(result == &buffer[0x42]);
    assert(*(unsigned char *)result == 0x42);
    
    /* Test finding first occurrence */
    memset(buffer, 0xAA, TEST_SIZE);
    buffer[100] = 0xBB;
    buffer[200] = 0xBB;
    
    result = memchr(buffer, 0xBB, TEST_SIZE);
    assert(result == &buffer[100]);
    
    /* Test not finding byte */
    result = memchr(buffer, 0xCC, TEST_SIZE);
    assert(result == NULL);
    
    /* Test finding at beginning */
    buffer[0] = 0xDD;
    result = memchr(buffer, 0xDD, TEST_SIZE);
    assert(result == &buffer[0]);
    
    /* Test finding at end */
    buffer[TEST_SIZE - 1] = 0xEE;
    result = memchr(buffer, 0xEE, TEST_SIZE);
    assert(result == &buffer[TEST_SIZE - 1]);
    
    /* Test zero length */
    result = memchr(buffer, 0xAA, 0);
    assert(result == NULL);
    
    /* Test limited search */
    memset(buffer, 0, TEST_SIZE);
    buffer[50] = 0xFF;
    
    result = memchr(buffer, 0xFF, 40);
    assert(result == NULL); /* Not found in first 40 bytes */
    
    result = memchr(buffer, 0xFF, 60);
    assert(result == &buffer[50]); /* Found when search includes byte 50 */
    
    /* Test null character search */
    strcpy((char *)buffer, "Hello, World!");
    result = memchr(buffer, '\0', 20);
    assert(result != NULL);
    assert(result == &buffer[13]); /* Position of null terminator */
    
    /* Test signed char values */
    buffer[0] = 0xFF;
    result = memchr(buffer, 0xFF, TEST_SIZE);
    assert(result == &buffer[0]);
    
    /* Test alignment */
    for (i = 0; i < 8; i++) {
        memset(buffer, 0, TEST_SIZE);
        buffer[i + 32] = 0x5A;
        
        result = memchr(buffer + i, 0x5A, 64);
        assert(result == &buffer[i + 32]);
    }
    
    /* Test multiple identical bytes */
    memset(buffer, 0x33, TEST_SIZE);
    result = memchr(buffer, 0x33, TEST_SIZE);
    assert(result == &buffer[0]); /* Should find first one */
    
    TEST_PASSED("memchr()");
}

/* Test edge cases and patterns */
void test_memory_ops_edge_cases(void) {
    unsigned char *buf1, *buf2;
    size_t i;
    
    TEST_START("memory operations edge cases");
    
    /* Test operations on NULL with zero size (undefined but shouldn't crash) */
    /* Note: These are technically undefined behavior */
    
    /* Test very large operations */
    size_t large_size = 10 * 1024 * 1024; /* 10MB */
    buf1 = (unsigned char *)malloc(large_size);
    buf2 = (unsigned char *)malloc(large_size);
    
    if (buf1 && buf2) {
        /* Large memset */
        memset(buf1, 0x55, large_size);
        assert(buf1[0] == 0x55);
        assert(buf1[large_size/2] == 0x55);
        assert(buf1[large_size-1] == 0x55);
        
        /* Large memcpy */
        memcpy(buf2, buf1, large_size);
        assert(buf2[0] == 0x55);
        assert(buf2[large_size/2] == 0x55);
        assert(buf2[large_size-1] == 0x55);
        
        /* Large memcmp */
        assert(memcmp(buf1, buf2, large_size) == 0);
        
        free(buf1);
        free(buf2);
    }
    
    /* Test page boundary operations */
    size_t page_size = 4096;
    buf1 = (unsigned char *)malloc(page_size * 2);
    
    if (buf1) {
        /* Operation crossing page boundary */
        memset(buf1 + page_size - 10, 0xAB, 20);
        
        for (i = page_size - 10; i < page_size + 10; i++) {
            assert(buf1[i] == 0xAB);
        }
        
        free(buf1);
    }
    
    /* Test all byte values */
    unsigned char all_bytes[256];
    unsigned char all_copy[256];
    
    for (i = 0; i < 256; i++) {
        all_bytes[i] = i;
    }
    
    memcpy(all_copy, all_bytes, 256);
    assert(memcmp(all_bytes, all_copy, 256) == 0);
    
    /* Test each byte with memchr */
    for (i = 0; i < 256; i++) {
        void *found = memchr(all_bytes, i, 256);
        assert(found == &all_bytes[i]);
    }
    
    TEST_PASSED("Edge case");
}

/* Test performance patterns */
void test_memory_ops_patterns(void) {
    TEST_START("memory operation patterns");
    
    /* Pattern 1: Clearing sensitive data */
    char password[] = "secret123";
    size_t len = strlen(password);
    
    /* Clear password from memory */
    memset(password, 0, len);
    
    size_t i;
    for (i = 0; i < len; i++) {
        assert(password[i] == 0);
    }
    
    /* Pattern 2: Buffer initialization */
    typedef struct {
        char header[16];
        int data[256];
        char footer[16];
    } Buffer;
    
    Buffer *buf = (Buffer *)malloc(sizeof(Buffer));
    if (buf) {
        memset(buf, 0, sizeof(Buffer));
        assert(buf->data[0] == 0);
        assert(buf->data[255] == 0);
        free(buf);
    }
    
    /* Pattern 3: Finding delimiters */
    unsigned char data[] = "field1|field2|field3";
    void *delim = memchr(data, '|', sizeof(data));
    assert(delim != NULL);
    assert(delim == &data[6]);
    
    /* Pattern 4: Comparing headers/magic numbers */
    unsigned char magic1[] = {0x89, 'P', 'N', 'G'};
    unsigned char magic2[] = {0x89, 'P', 'N', 'G'};
    unsigned char magic3[] = {0xFF, 0xD8, 0xFF, 0xE0};
    
    assert(memcmp(magic1, magic2, 4) == 0);
    assert(memcmp(magic1, magic3, 4) != 0);
    
    /* Pattern 5: Circular buffer rotation */
    unsigned char circular[10] = {0,1,2,3,4,5,6,7,8,9};
    unsigned char temp[3];
    
    /* Rotate by 3 positions */
    memcpy(temp, circular, 3);
    memmove(circular, circular + 3, 7);
    memcpy(circular + 7, temp, 3);
    
    /* Check rotation: should be 3,4,5,6,7,8,9,0,1,2 */
    for (i = 0; i < 10; i++) {
        assert(circular[i] == (i + 3) % 10);
    }
    
    TEST_PASSED("Pattern");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 string.h Memory Operations Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_memset();
    test_memcpy();
    test_memmove();
    test_memcmp();
    test_memchr();
    test_memory_ops_edge_cases();
    test_memory_ops_patterns();
    
    TEST_SUITE_PASSED("memory operation");
    return 0;
}
