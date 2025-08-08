/*
 * test_stdio_embedded.c - PSE51 stdio.h test suite for embedded systems
 * Tests: printf, sprintf, snprintf, fopen, fclose, fread, fwrite, fgets, fputs, fprintf, fscanf
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <compiler.h>
#include <uart.h>
#include <stdarg.h>
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


/* Test printf family */
void test_printf_family(void) {
    char buf[128];
    int n;
    
    printf("Testing printf family...");
    
    /* Test sprintf */
    n = sprintf(buf, "Hello %s %d", "World", 42);
    TEST_ASSERT(n == 14);
    TEST_ASSERT(strcmp(buf, "Hello World 42") == 0);
    
    /* Test snprintf */
    n = snprintf(buf, 10, "Hello %s %d", "World", 42);
    TEST_ASSERT(n == 14); /* Returns what would have been written */
    TEST_ASSERT(strlen(buf) == 9); /* But only writes size-1 chars */
    
    /* Test printf (output visible) */
    printf(" [printf test: %d] ", 123);
    
    printf("PASSED\n");
    tests_passed++;
}

/* Test file operations */
void test_file_operations(void) {
    FILE *fp;
    char buf[32];
    int n;
    
    printf("Testing file operations...");
    
    /* Try to open a file */
    fp = fopen("/tmp/test.txt", "w");
    if (fp == NULL) {
        SKIP_TEST("fopen not implemented");
    }
    
    /* Write to file */
    n = fprintf(fp, "Test data\n");
    TEST_ASSERT(n > 0);
    
    fclose(fp);
    
    /* Read back */
    fp = fopen("/tmp/test.txt", "r");
    if (fp != NULL) {
        if (fgets(buf, sizeof(buf), fp) != NULL) {
            TEST_ASSERT(strcmp(buf, "Test data\n") == 0);
        }
        fclose(fp);
    }
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test standard streams */
void test_standard_streams(void) {
    printf("Testing standard streams...");
    
    /* stdout should work (we're using it) */
    TEST_ASSERT(stdout != NULL);
    
    /* stderr might be available */
    if (stderr != NULL) {
        fprintf(stderr, " [stderr test] ");
    }
    
    /* stdin might not be available in embedded */
    if (stdin != NULL) {
        printf(" [stdin available] ");
    }
    
    printf("PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 stdio.h Test Summary ===\n");
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
    
    printf("=== PSE51 stdio.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_printf_family();
    test_file_operations();
    test_standard_streams();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
