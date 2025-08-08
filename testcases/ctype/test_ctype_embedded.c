/*
 * test_ctype_embedded.c - PSE51 ctype.h test suite for embedded systems
 * Tests: isalpha, isdigit, isalnum, islower, isupper, isspace, isprint,
 *        isgraph, iscntrl, ispunct, isxdigit, tolower, toupper
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <compiler.h>
#include <uart.h>
#include <sys/types.h>

/* Global test status */
static int tests_passed = 0;
static int tests_failed = 0;

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
    


/* Test helper macro */
#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        printf("\n  FAILED: %s at line %d", #cond, __LINE__); \
        tests_failed++; \
    } \
} while(0)

/* Test isalpha() */
void test_isalpha(void) {
    int c;
    
    printf("Testing isalpha()...");
    
    /* Test uppercase letters */
    for (c = 'A'; c <= 'Z'; c++) {
        TEST_ASSERT(isalpha(c) != 0);
    }
    
    /* Test lowercase letters */
    for (c = 'a'; c <= 'z'; c++) {
        TEST_ASSERT(isalpha(c) != 0);
    }
    
    /* Test non-letters */
    for (c = '0'; c <= '9'; c++) {
        TEST_ASSERT(isalpha(c) == 0);
    }
    
    TEST_ASSERT(isalpha(' ') == 0);
    TEST_ASSERT(isalpha('\n') == 0);
    TEST_ASSERT(isalpha('@') == 0);
    TEST_ASSERT(isalpha('[') == 0);
    TEST_ASSERT(isalpha('`') == 0);
    TEST_ASSERT(isalpha('{') == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test isdigit() */
void test_isdigit(void) {
    int c;
    
    printf("Testing isdigit()...");
    
    /* Test digits */
    for (c = '0'; c <= '9'; c++) {
        TEST_ASSERT(isdigit(c) != 0);
    }
    
    /* Test non-digits */
    for (c = 'A'; c <= 'Z'; c++) {
        TEST_ASSERT(isdigit(c) == 0);
    }
    for (c = 'a'; c <= 'z'; c++) {
        TEST_ASSERT(isdigit(c) == 0);
    }
    
    TEST_ASSERT(isdigit(' ') == 0);
    TEST_ASSERT(isdigit('\n') == 0);
    TEST_ASSERT(isdigit('+') == 0);
    TEST_ASSERT(isdigit('-') == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test isalnum() */
void test_isalnum(void) {
    int c;
    
    printf("Testing isalnum()...");
    
    /* Letters and digits should return true */
    for (c = 'A'; c <= 'Z'; c++) {
        TEST_ASSERT(isalnum(c) != 0);
    }
    for (c = 'a'; c <= 'z'; c++) {
        TEST_ASSERT(isalnum(c) != 0);
    }
    for (c = '0'; c <= '9'; c++) {
        TEST_ASSERT(isalnum(c) != 0);
    }
    
    /* Special characters should return false */
    TEST_ASSERT(isalnum(' ') == 0);
    TEST_ASSERT(isalnum('!') == 0);
    TEST_ASSERT(isalnum('@') == 0);
    TEST_ASSERT(isalnum('#') == 0);
    TEST_ASSERT(isalnum('$') == 0);
    TEST_ASSERT(isalnum('%') == 0);
    TEST_ASSERT(isalnum('^') == 0);
    TEST_ASSERT(isalnum('&') == 0);
    TEST_ASSERT(isalnum('*') == 0);
    TEST_ASSERT(isalnum('(') == 0);
    TEST_ASSERT(isalnum(')') == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test islower() and isupper() */
void test_case_functions(void) {
    int c;
    
    printf("Testing islower() and isupper()...");
    
    /* Test islower */
    for (c = 'a'; c <= 'z'; c++) {
        TEST_ASSERT(islower(c) != 0);
        TEST_ASSERT(isupper(c) == 0);
    }
    
    /* Test isupper */
    for (c = 'A'; c <= 'Z'; c++) {
        TEST_ASSERT(isupper(c) != 0);
        TEST_ASSERT(islower(c) == 0);
    }
    
    /* Test non-letters */
    for (c = '0'; c <= '9'; c++) {
        TEST_ASSERT(islower(c) == 0);
        TEST_ASSERT(isupper(c) == 0);
    }
    
    TEST_ASSERT(islower(' ') == 0);
    TEST_ASSERT(isupper(' ') == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test isspace() */
void test_isspace(void) {
    printf("Testing isspace()...");
    
    /* Standard space characters */
    TEST_ASSERT(isspace(' ') != 0);
    TEST_ASSERT(isspace('\t') != 0);
    TEST_ASSERT(isspace('\n') != 0);
    TEST_ASSERT(isspace('\v') != 0);
    TEST_ASSERT(isspace('\f') != 0);
    TEST_ASSERT(isspace('\r') != 0);
    
    /* Non-space characters */
    TEST_ASSERT(isspace('A') == 0);
    TEST_ASSERT(isspace('0') == 0);
    TEST_ASSERT(isspace('!') == 0);
    TEST_ASSERT(isspace('\0') == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test isprint() and isgraph() */
void test_print_graph(void) {
    int c;
    
    printf("Testing isprint() and isgraph()...");
    
    /* Printable ASCII characters */
    for (c = 32; c <= 126; c++) {
        TEST_ASSERT(isprint(c) != 0);
        if (c != ' ') {
            TEST_ASSERT(isgraph(c) != 0);
        } else {
            TEST_ASSERT(isgraph(c) == 0);
        }
    }
    
    /* Control characters */
    for (c = 0; c < 32; c++) {
        TEST_ASSERT(isprint(c) == 0);
        TEST_ASSERT(isgraph(c) == 0);
    }
    
    TEST_ASSERT(isprint(127) == 0);
    TEST_ASSERT(isgraph(127) == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test iscntrl() */
void test_iscntrl(void) {
    int c;
    
    printf("Testing iscntrl()...");
    
    /* Control characters */
    for (c = 0; c < 32; c++) {
        TEST_ASSERT(iscntrl(c) != 0);
    }
    TEST_ASSERT(iscntrl(127) != 0);
    
    /* Non-control characters */
    for (c = 32; c <= 126; c++) {
        TEST_ASSERT(iscntrl(c) == 0);
    }
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test ispunct() */
void test_ispunct(void) {
    const char *punct = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    const char *p;
    int c;
    
    printf("Testing ispunct()...");
    
    /* Test known punctuation */
    for (p = punct; *p; p++) {
        TEST_ASSERT(ispunct(*p) != 0);
    }
    
    /* Test non-punctuation */
    for (c = 'A'; c <= 'Z'; c++) {
        TEST_ASSERT(ispunct(c) == 0);
    }
    for (c = 'a'; c <= 'z'; c++) {
        TEST_ASSERT(ispunct(c) == 0);
    }
    for (c = '0'; c <= '9'; c++) {
        TEST_ASSERT(ispunct(c) == 0);
    }
    TEST_ASSERT(ispunct(' ') == 0);
    TEST_ASSERT(ispunct('\n') == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test isxdigit() */
void test_isxdigit(void) {
    int c;
    
    printf("Testing isxdigit()...");
    
    /* Decimal digits */
    for (c = '0'; c <= '9'; c++) {
        TEST_ASSERT(isxdigit(c) != 0);
    }
    
    /* Hex digits A-F */
    for (c = 'A'; c <= 'F'; c++) {
        TEST_ASSERT(isxdigit(c) != 0);
    }
    
    /* Hex digits a-f */
    for (c = 'a'; c <= 'f'; c++) {
        TEST_ASSERT(isxdigit(c) != 0);
    }
    
    /* Non-hex characters */
    for (c = 'G'; c <= 'Z'; c++) {
        TEST_ASSERT(isxdigit(c) == 0);
    }
    for (c = 'g'; c <= 'z'; c++) {
        TEST_ASSERT(isxdigit(c) == 0);
    }
    TEST_ASSERT(isxdigit(' ') == 0);
    TEST_ASSERT(isxdigit('!') == 0);
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test tolower() and toupper() */
void test_case_conversion(void) {
    int c;
    
    printf("Testing tolower() and toupper()...");
    
    /* Test tolower */
    for (c = 'A'; c <= 'Z'; c++) {
        TEST_ASSERT(tolower(c) == c + 32);
    }
    for (c = 'a'; c <= 'z'; c++) {
        TEST_ASSERT(tolower(c) == c);
    }
    
    /* Test toupper */
    for (c = 'a'; c <= 'z'; c++) {
        TEST_ASSERT(toupper(c) == c - 32);
    }
    for (c = 'A'; c <= 'Z'; c++) {
        TEST_ASSERT(toupper(c) == c);
    }
    
    /* Non-letters should be unchanged */
    for (c = '0'; c <= '9'; c++) {
        TEST_ASSERT(tolower(c) == c);
        TEST_ASSERT(toupper(c) == c);
    }
    
    TEST_ASSERT(tolower(' ') == ' ');
    TEST_ASSERT(toupper(' ') == ' ');
    TEST_ASSERT(tolower('!') == '!');
    TEST_ASSERT(toupper('!') == '!');
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test character class relationships */
void test_relationships(void) {
    int c;
    
    printf("Testing character class relationships...");
    
    /* If isalpha, then isalnum */
    for (c = 0; c < 128; c++) {
        if (isalpha(c)) {
            TEST_ASSERT(isalnum(c) != 0);
        }
    }
    
    /* If isdigit, then isalnum */
    for (c = 0; c < 128; c++) {
        if (isdigit(c)) {
            TEST_ASSERT(isalnum(c) != 0);
        }
    }
    
    /* If isupper, then isalpha */
    for (c = 0; c < 128; c++) {
        if (isupper(c)) {
            TEST_ASSERT(isalpha(c) != 0);
        }
    }
    
    /* If islower, then isalpha */
    for (c = 0; c < 128; c++) {
        if (islower(c)) {
            TEST_ASSERT(isalpha(c) != 0);
        }
    }
    
    /* Control characters are not printable */
    for (c = 0; c < 128; c++) {
        if (iscntrl(c)) {
            TEST_ASSERT(isprint(c) == 0);
        }
    }
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Test edge cases */
void test_edge_cases(void) {
    printf("Testing edge cases...");
    
    /* Test EOF */
    TEST_ASSERT(isalpha(EOF) == 0);
    TEST_ASSERT(isdigit(EOF) == 0);
    TEST_ASSERT(isalnum(EOF) == 0);
    TEST_ASSERT(islower(EOF) == 0);
    TEST_ASSERT(isupper(EOF) == 0);
    TEST_ASSERT(isspace(EOF) == 0);
    TEST_ASSERT(isprint(EOF) == 0);
    TEST_ASSERT(isgraph(EOF) == 0);
    TEST_ASSERT(iscntrl(EOF) == 0);
    TEST_ASSERT(ispunct(EOF) == 0);
    TEST_ASSERT(isxdigit(EOF) == 0);
    
    /* tolower/toupper should return EOF unchanged */
    TEST_ASSERT(tolower(EOF) == EOF);
    TEST_ASSERT(toupper(EOF) == EOF);
    
    /* Test negative values (common with signed char) */
    TEST_ASSERT(isalpha(-1) == 0);
    TEST_ASSERT(isdigit(-1) == 0);
    
    /* Test high values */
    if (CHAR_BIT == 8) {
        TEST_ASSERT(isalpha(128) == 0);
        TEST_ASSERT(isdigit(255) == 0);
    }
    
    if (tests_failed == 0) {
        printf(" PASSED\n");
        tests_passed++;
    } else {
        printf("\n");
    }
}

/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 ctype.h Test Summary ===\n");
    printf("=====================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    
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
    
    printf("=== PSE51 ctype.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    
    /* Run tests */
    test_isalpha();
    test_isdigit();
    test_isalnum();
    test_case_functions();
    test_isspace();
    test_print_graph();
    test_iscntrl();
    test_ispunct();
    test_isxdigit();
    test_case_conversion();
    test_relationships();
    test_edge_cases();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
