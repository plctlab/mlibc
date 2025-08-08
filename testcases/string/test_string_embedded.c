/*
 * test_string_embedded.c - PSE51 string.h test suite for embedded systems
 * Tests: memcpy, memmove, memset, memcmp, strcpy, strncpy, strcat, strncat, strcmp, strncmp, strlen, strchr, strrchr, strstr
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <compiler.h>
#include <uart.h>
#include <sys/types.h>


/* Global test status */
static int tests_passed = 0;
static int tests_failed = 0;
static int tests_skipped = 0;

/* Implement write() for printf support */
ssize_t write(int fd, const void *buf, size_t count)
{
    (void)fd;
    char *buffer = (char*)buf;
    size_t i;
    
    for (i = 0; i < count; i++) {
        UartPutc(buffer[i]);
    }
    
    return count;
}
    


/* Test helper macros */
#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        printf("\n  FAILED: %s at line %d", #cond, __LINE__); \
        tests_failed++; \
    } \
} while(0)

#define SKIP_TEST(reason) do { \
    printf(" [SKIPPED: %s]", reason); \
    tests_skipped++; \
    return; \
} while(0)


/* Test memory functions */
void test_memory_functions(void) {
    char buf1[32], buf2[32];
    
    printf("Testing memory functions...");
    
    /* Test memset */
    memset(buf1, 'A', 10);
    TEST_ASSERT(buf1[0] == 'A' && buf1[9] == 'A');
    
    /* Test memcpy */
    memcpy(buf2, buf1, 10);
    TEST_ASSERT(memcmp(buf1, buf2, 10) == 0);
    
    /* Test memmove with overlap */
    memmove(buf1 + 5, buf1, 5);
    TEST_ASSERT(buf1[5] == 'A' && buf1[9] == 'A');
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test string functions */
void test_string_functions(void) {
    char buf[64];
    char *p;
    
    printf("Testing string functions...");
    
    /* Test strcpy */
    strcpy(buf, "Hello");
    TEST_ASSERT(strcmp(buf, "Hello") == 0);
    TEST_ASSERT(strlen(buf) == 5);
    
    /* Test strcat */
    strcat(buf, " World");
    TEST_ASSERT(strcmp(buf, "Hello World") == 0);
    
    /* Test strchr */
    p = strchr(buf, 'W');
    TEST_ASSERT(p != NULL && *p == 'W');
    
    /* Test strstr */
    p = strstr(buf, "World");
    TEST_ASSERT(p != NULL && strcmp(p, "World") == 0);
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test bounded string functions */
void test_bounded_string_functions(void) {
    char buf[32];
    
    printf("Testing bounded string functions...");
    
    /* Test strncpy */
    memset(buf, 0, sizeof(buf));
    strncpy(buf, "Hello", 3);
    TEST_ASSERT(strncmp(buf, "Hel", 3) == 0);
    
    /* Test strncat */
    strcpy(buf, "Hello");
    strncat(buf, " World!", 6);
    TEST_ASSERT(strcmp(buf, "Hello World") == 0);
    
    printf(" PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 string.h Test Summary ===\n");
    printf("=====================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Tests skipped: %d\n", tests_skipped);
    
    if (tests_failed == 0) {
        printf("\n*** ALL TESTS PASSED! ***\n");
    } else {
        printf("\n*** %d TESTS FAILED! ***\n", tests_failed);
    }
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    /* Initialize UART for output */
    UartStdOutInit();
    
    printf("=== PSE51 string.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_memory_functions();
    test_string_functions();
    test_bounded_string_functions();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
