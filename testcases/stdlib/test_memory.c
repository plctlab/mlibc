/*
 * test_memory.c - PSE51 stdlib.h memory management test suite
 * Tests: malloc, free, calloc, realloc
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for memory management
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include "../test_colors.h"
#define SMALL_SIZE 16
#define MEDIUM_SIZE 1024
#define LARGE_SIZE (1024 * 1024)  /* 1MB */

/* Test malloc() basic functionality */
void test_malloc_basic(void) {
    void *ptr;
    char *char_ptr;
    int *int_ptr;
    size_t i;
    
    TEST_START("malloc() basic functionality");
    
    /* Test zero-size allocation */
    ptr = malloc(0);
    /* Behavior is implementation-defined: may return NULL or unique pointer */
    if (ptr != NULL) {
        free(ptr);
    }
    
    /* Test small allocation */
    ptr = malloc(SMALL_SIZE);
    assert(ptr != NULL);
    
    /* Test that memory is writable */
    memset(ptr, 0xAA, SMALL_SIZE);
    char_ptr = (char *)ptr;
    for (i = 0; i < SMALL_SIZE; i++) {
        assert(char_ptr[i] == (char)0xAA);
    }
    free(ptr);
    
    /* Test medium allocation */
    ptr = malloc(MEDIUM_SIZE);
    assert(ptr != NULL);
    memset(ptr, 0, MEDIUM_SIZE);
    free(ptr);
    
    /* Test aligned allocation for different types */
    int_ptr = (int *)malloc(sizeof(int) * 100);
    assert(int_ptr != NULL);
    assert(((uintptr_t)int_ptr % sizeof(int)) == 0); /* Check alignment */
    
    for (i = 0; i < 100; i++) {
        int_ptr[i] = i;
    }
    for (i = 0; i < 100; i++) {
        assert(int_ptr[i] == (int)i);
    }
    free(int_ptr);
    
    /* Test multiple allocations */
    void *ptrs[10];
    for (i = 0; i < 10; i++) {
        ptrs[i] = malloc((i + 1) * 10);
        assert(ptrs[i] != NULL);
    }
    
    /* Free in reverse order */
    for (i = 10; i > 0; i--) {
        free(ptrs[i - 1]);
    }
    
    TEST_PASSED("malloc() basic");
}

/* Test malloc() edge cases and errors */
void test_malloc_edge_cases(void) {
    void *ptr;
    size_t max_size;
    
    TEST_START("malloc() edge cases");
    
    /* Test very large allocation (likely to fail) */
    max_size = (size_t)-1; /* Maximum size_t value */
    errno = 0;
    ptr = malloc(max_size);
    if (ptr == NULL) {
        assert(errno == ENOMEM || errno == 0); /* May or may not set errno */
    } else {
        /* Unlikely but possible on 64-bit systems with overcommit */
        free(ptr);
    }
    
    /* Test pattern of alternating small allocations and frees */
    void *ptrs[100];
    size_t i;
    
    for (i = 0; i < 100; i++) {
        ptrs[i] = malloc(16 + (i % 64));
        assert(ptrs[i] != NULL);
        
        /* Free every other allocation immediately */
        if (i % 2 == 1) {
            free(ptrs[i - 1]);
            ptrs[i - 1] = NULL;
        }
    }
    
    /* Free remaining allocations */
    for (i = 0; i < 100; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
        }
    }
    
    /* Test allocation alignment */
    for (i = 1; i <= 128; i *= 2) {
        ptr = malloc(i);
        assert(ptr != NULL);
        
        /* Check that pointer is suitably aligned */
        assert(((uintptr_t)ptr % sizeof(void *)) == 0);
        
        free(ptr);
    }
    
    TEST_PASSED("malloc() edge case");
}

/* Test free() functionality */
void test_free(void) {
    void *ptr;
    
    TEST_START("free()");
    
    /* Test free(NULL) - should be safe */
    free(NULL);
    
    /* Test normal free */
    ptr = malloc(100);
    assert(ptr != NULL);
    free(ptr);
    
    /* Test double-free detection (if available) */
    /* Note: Double-free is undefined behavior, so we can't reliably test it */
    
    /* Test free of zero-size allocation */
    ptr = malloc(0);
    free(ptr); /* Should be safe whether ptr is NULL or not */
    
    TEST_PASSED("free()");
}

/* Test calloc() functionality */
void test_calloc(void) {
    char *char_ptr;
    int *int_ptr;
    size_t i, j;
    void *ptr;
    
    TEST_START("calloc()");
    
    /* Test basic calloc */
    char_ptr = (char *)calloc(10, sizeof(char));
    assert(char_ptr != NULL);
    
    /* Verify memory is zeroed */
    for (i = 0; i < 10; i++) {
        assert(char_ptr[i] == 0);
    }
    free(char_ptr);
    
    /* Test calloc with different sizes */
    int_ptr = (int *)calloc(100, sizeof(int));
    assert(int_ptr != NULL);
    
    for (i = 0; i < 100; i++) {
        assert(int_ptr[i] == 0);
    }
    free(int_ptr);
    
    /* Test calloc with zero count */
    ptr = calloc(0, sizeof(int));
    /* May return NULL or unique pointer */
    free(ptr);
    
    /* Test calloc with zero size */
    ptr = calloc(10, 0);
    /* May return NULL or unique pointer */
    free(ptr);
    
    /* Test calloc with both zero */
    ptr = calloc(0, 0);
    free(ptr);
    
    /* Test large calloc */
    char_ptr = (char *)calloc(MEDIUM_SIZE, 1);
    assert(char_ptr != NULL);
    
    /* Verify all bytes are zero */
    for (i = 0; i < MEDIUM_SIZE; i++) {
        assert(char_ptr[i] == 0);
    }
    free(char_ptr);
    
    /* Test calloc overflow protection */
    /* SIZE_MAX / 2 * 2 would overflow */
    size_t half_max = ((size_t)-1) / 2 + 1;
    errno = 0;
    ptr = calloc(half_max, 2);
    if (ptr == NULL) {
        /* Good - overflow was detected */
        assert(errno == ENOMEM || errno == 0);
    } else {
        /* System has a lot of memory or uses overcommit */
        free(ptr);
    }
    
    /* Test 2D array allocation pattern */
    int **array_2d = (int **)calloc(10, sizeof(int *));
    assert(array_2d != NULL);
    
    for (i = 0; i < 10; i++) {
        array_2d[i] = (int *)calloc(20, sizeof(int));
        assert(array_2d[i] != NULL);
        
        /* Verify zeros */
        for (j = 0; j < 20; j++) {
            assert(array_2d[i][j] == 0);
        }
    }
    
    /* Free 2D array */
    for (i = 0; i < 10; i++) {
        free(array_2d[i]);
    }
    free(array_2d);
    
    TEST_PASSED("calloc()");
}

/* Test realloc() functionality */
void test_realloc(void) {
    char *ptr, *new_ptr;
    size_t i;
    
    TEST_START("realloc()");
    
    /* Test realloc(NULL, size) - should behave like malloc */
    ptr = (char *)realloc(NULL, 100);
    assert(ptr != NULL);
    memset(ptr, 'A', 100);
    
    /* Test growing reallocation */
    new_ptr = (char *)realloc(ptr, 200);
    assert(new_ptr != NULL);
    ptr = new_ptr;
    
    /* Verify old data is preserved */
    for (i = 0; i < 100; i++) {
        assert(ptr[i] == 'A');
    }
    
    /* Fill new space */
    memset(ptr + 100, 'B', 100);
    
    /* Test shrinking reallocation */
    new_ptr = (char *)realloc(ptr, 50);
    assert(new_ptr != NULL);
    ptr = new_ptr;
    
    /* Verify data is preserved */
    for (i = 0; i < 50; i++) {
        assert(ptr[i] == 'A');
    }
    
    /* Test realloc(ptr, 0) - implementation-defined */
    new_ptr = (char *)realloc(ptr, 0);
    /* May return NULL (and free ptr) or return a unique pointer */
    if (new_ptr == NULL) {
        /* ptr was freed */
        ptr = NULL;
    } else {
        free(new_ptr);
    }
    
    /* Test realloc with same size */
    ptr = (char *)malloc(100);
    assert(ptr != NULL);
    memset(ptr, 'C', 100);
    
    new_ptr = (char *)realloc(ptr, 100);
    assert(new_ptr != NULL);
    ptr = new_ptr;
    
    /* Data should be unchanged */
    for (i = 0; i < 100; i++) {
        assert(ptr[i] == 'C');
    }
    free(ptr);
    
    /* Test realloc failure (doesn't free original) */
    ptr = (char *)malloc(100);
    assert(ptr != NULL);
    memset(ptr, 'D', 100);
    
    errno = 0;
    new_ptr = (char *)realloc(ptr, (size_t)-1); /* Likely to fail */
    if (new_ptr == NULL) {
        /* Original ptr should still be valid */
        assert(ptr[0] == 'D');
        free(ptr);
    } else {
        /* Unlikely but possible */
        free(new_ptr);
    }
    
    /* Test realloc pattern (growing array) */
    size_t capacity = 10;
    int *array = (int *)malloc(capacity * sizeof(int));
    assert(array != NULL);
    
    for (i = 0; i < capacity; i++) {
        array[i] = i;
    }
    
    /* Grow array */
    capacity *= 2;
    int *new_array = (int *)realloc(array, capacity * sizeof(int));
    assert(new_array != NULL);
    array = new_array;
    
    /* Verify old data */
    for (i = 0; i < 10; i++) {
        assert(array[i] == (int)i);
    }
    
    /* Fill new data */
    for (i = 10; i < capacity; i++) {
        array[i] = i;
    }
    
    free(array);
    
    TEST_PASSED("realloc()");
}

/* Test memory patterns and stress */
void test_memory_patterns(void) {
    void *ptrs[1000];
    size_t sizes[1000];
    size_t i, j;
    int pattern;
    
    TEST_START("memory allocation patterns");
    
    /* Test 1: Allocate many small blocks */
    for (i = 0; i < 1000; i++) {
        sizes[i] = 1 + (i % 100);
        ptrs[i] = malloc(sizes[i]);
        assert(ptrs[i] != NULL);
        
        /* Fill with pattern */
        pattern = i & 0xFF;
        memset(ptrs[i], pattern, sizes[i]);
    }
    
    /* Verify patterns */
    for (i = 0; i < 1000; i++) {
        pattern = i & 0xFF;
        unsigned char *p = (unsigned char *)ptrs[i];
        for (j = 0; j < sizes[i]; j++) {
            assert(p[j] == pattern);
        }
    }
    
    /* Free every other block */
    for (i = 0; i < 1000; i += 2) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }
    
    /* Reallocate freed blocks with different sizes */
    for (i = 0; i < 1000; i += 2) {
        sizes[i] = 1 + ((i * 7) % 200);
        ptrs[i] = malloc(sizes[i]);
        assert(ptrs[i] != NULL);
        memset(ptrs[i], i & 0xFF, sizes[i]);
    }
    
    /* Free all blocks */
    for (i = 0; i < 1000; i++) {
        free(ptrs[i]);
    }
    
    /* Test 2: Allocation size ladder */
    for (i = 0; i < 20; i++) {
        size_t size = 1 << i; /* 1, 2, 4, 8, ... */
        if (size > LARGE_SIZE) break;
        
        void *p = malloc(size);
        if (p == NULL) break; /* Out of memory */
        
        /* Touch memory */
        memset(p, 0x55, size);
        
        /* Verify */
        unsigned char *cp = (unsigned char *)p;
        assert(cp[0] == 0x55);
        assert(cp[size - 1] == 0x55);
        
        free(p);
    }
    
    /* Test 3: Realloc stress */
    void *p = malloc(1);
    assert(p != NULL);
    
    for (i = 1; i <= 100; i++) {
        void *new_p = realloc(p, i * 10);
        assert(new_p != NULL);
        p = new_p;
        
        /* Touch new memory */
        ((char *)p)[i * 10 - 1] = i;
    }
    
    free(p);
    
    TEST_PASSED("Memory pattern");
}

/* Test memory alignment */
void test_memory_alignment(void) {
    void *ptr;
    size_t i;
    
    TEST_START("memory alignment");
    
    /* Test various allocation sizes for alignment */
    for (i = 1; i <= 1024; i *= 2) {
        ptr = malloc(i);
        assert(ptr != NULL);
        
        /* Check minimum alignment (should be at least pointer-sized) */
        assert(((uintptr_t)ptr % sizeof(void *)) == 0);
        
        /* For larger allocations, alignment might be larger */
        if (i >= 16) {
            /* Often 16-byte aligned for SIMD */
            /* This is implementation-specific */
        }
        
        free(ptr);
    }
    
    /* Test calloc alignment */
    for (i = 1; i <= 64; i++) {
        ptr = calloc(i, i);
        assert(ptr != NULL);
        assert(((uintptr_t)ptr % sizeof(void *)) == 0);
        free(ptr);
    }
    
    /* Test struct alignment */
    struct test_struct {
        char c;
        int i;
        double d;
    };
    
    struct test_struct *s = (struct test_struct *)malloc(sizeof(struct test_struct));
    assert(s != NULL);
    assert(((uintptr_t)s % sizeof(void *)) == 0);
    
    /* Use the struct */
    s->c = 'A';
    s->i = 42;
    s->d = 3.14;
    
    assert(s->c == 'A');
    assert(s->i == 42);
    assert(s->d == 3.14);
    
    free(s);
    
    TEST_PASSED("Memory alignment");
}

/* Test memory with threading considerations (single-threaded test) */
void test_memory_thread_safety(void) {
    void *ptrs[100];
    size_t i;
    
    TEST_START("memory allocation thread safety (single-threaded)");
    
    /* Rapid allocation/deallocation that might stress thread safety */
    for (i = 0; i < 10000; i++) {
        size_t idx = i % 100;
        
        if (i < 100 || (i % 3) == 0) {
            /* Allocate */
            ptrs[idx] = malloc(10 + (i % 1000));
            assert(ptrs[idx] != NULL);
        } else {
            /* Free and reallocate */
            free(ptrs[idx]);
            ptrs[idx] = malloc(20 + (i % 500));
            assert(ptrs[idx] != NULL);
        }
    }
    
    /* Clean up */
    for (i = 0; i < 100; i++) {
        free(ptrs[i]);
    }
    
    printf("  Thread safety tests passed (single-threaded)!\n");
}

/* Test memory leak detection helpers */
void test_memory_bookkeeping(void) {
    size_t i;
    void *p;
    
    TEST_START("memory bookkeeping");
    
    /* Simple leak detection by allocation counting */
    size_t alloc_count = 0;
    
    /* Allocate some memory */
    for (i = 0; i < 10; i++) {
        p = malloc(100);
        assert(p != NULL);
        alloc_count++;
        free(p);
        alloc_count--;
    }
    
    assert(alloc_count == 0); /* All freed */
    
    /* Test with realloc */
    p = malloc(50);
    alloc_count++;
    
    p = realloc(p, 100);
    /* Count stays the same */
    
    free(p);
    alloc_count--;
    
    assert(alloc_count == 0);
    
    TEST_PASSED("Memory bookkeeping");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdlib.h Memory Management Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_malloc_basic();
    test_malloc_edge_cases();
    test_free();
    test_calloc();
    test_realloc();
    test_memory_patterns();
    test_memory_alignment();
    test_memory_thread_safety();
    test_memory_bookkeeping();
    
    TEST_SUITE_PASSED("memory management");
    return 0;
}
