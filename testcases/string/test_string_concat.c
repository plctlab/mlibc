/*
 * test_string_concat.c - PSE51 string.h string concatenation test suite
 * Tests: strcat, strncat
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for string concatenation functions
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "../test_colors.h"
#define BUFFER_SIZE 1024

/* Test strcat() function */
void test_strcat(void) {
    char dst[BUFFER_SIZE];
    char *result;
    size_t i;
    
    TEST_START("strcat()");
    
    /* Test basic concatenation */
    strcpy(dst, "Hello");
    result = strcat(dst, ", World!");
    assert(result == dst);
    assert(strcmp(dst, "Hello, World!") == 0);
    assert(strlen(dst) == 13);
    
    /* Test concatenating empty string */
    strcpy(dst, "Test");
    result = strcat(dst, "");
    assert(result == dst);
    assert(strcmp(dst, "Test") == 0);
    
    /* Test concatenating to empty string */
    strcpy(dst, "");
    result = strcat(dst, "Hello");
    assert(result == dst);
    assert(strcmp(dst, "Hello") == 0);
    
    /* Test multiple concatenations */
    strcpy(dst, "One");
    strcat(dst, " ");
    strcat(dst, "Two");
    strcat(dst, " ");
    strcat(dst, "Three");
    assert(strcmp(dst, "One Two Three") == 0);
    
    /* Test concatenating single character */
    strcpy(dst, "Test");
    strcat(dst, "!");
    assert(strcmp(dst, "Test!") == 0);
    
    /* Test concatenating long strings */
    strcpy(dst, "Start");
    char long_str[100];
    for (i = 0; i < 99; i++) {
        long_str[i] = 'A' + (i % 26);
    }
    long_str[99] = '\0';
    
    strcat(dst, long_str);
    assert(strlen(dst) == 5 + 99);
    assert(memcmp(dst, "Start", 5) == 0);
    assert(memcmp(dst + 5, long_str, 99) == 0);
    
    /* Test special characters */
    strcpy(dst, "Line1");
    strcat(dst, "\n");
    strcat(dst, "Line2");
    strcat(dst, "\t");
    strcat(dst, "Tab");
    assert(strcmp(dst, "Line1\nLine2\tTab") == 0);
    
    /* Test building a path */
    strcpy(dst, "/home");
    strcat(dst, "/");
    strcat(dst, "user");
    strcat(dst, "/");
    strcat(dst, "file.txt");
    assert(strcmp(dst, "/home/user/file.txt") == 0);
    
    /* Test concatenating numbers as strings */
    strcpy(dst, "Number: ");
    strcat(dst, "123");
    strcat(dst, ".");
    strcat(dst, "456");
    assert(strcmp(dst, "Number: 123.456") == 0);
    
    /* Test all ASCII printable characters */
    strcpy(dst, "ASCII: ");
    char ascii[2] = {0, 0};
    for (i = 32; i < 127; i++) {
        ascii[0] = i;
        strcat(dst, ascii);
    }
    assert(strlen(dst) == 7 + 95);
    
    /* Test UTF-8 concatenation */
    strcpy(dst, "Hello");
    strcat(dst, ", ");
    strcat(dst, "世界");
    strcat(dst, "!");
    assert(strcmp(dst, "Hello, 世界!") == 0);
    
    TEST_PASSED("strcat()");
}

/* Test strncat() function */
void test_strncat(void) {
    char dst[BUFFER_SIZE];
    char *result;
    size_t i;
    
    TEST_START("strncat()");
    
    /* Test basic concatenation with exact length */
    strcpy(dst, "Hello");
    result = strncat(dst, ", World!", 8);
    assert(result == dst);
    assert(strcmp(dst, "Hello, World!") == 0);
    
    /* Test concatenation with limited length */
    strcpy(dst, "Hello");
    result = strncat(dst, ", World!", 3);
    assert(result == dst);
    assert(strcmp(dst, "Hello, W") == 0);
    assert(dst[8] == '\0'); /* Null terminated */
    
    /* Test n = 0 */
    strcpy(dst, "Test");
    result = strncat(dst, "Append", 0);
    assert(result == dst);
    assert(strcmp(dst, "Test") == 0);
    
    /* Test concatenating empty string */
    strcpy(dst, "Test");
    result = strncat(dst, "", 10);
    assert(result == dst);
    assert(strcmp(dst, "Test") == 0);
    
    /* Test concatenating to empty string */
    strcpy(dst, "");
    result = strncat(dst, "Hello", 5);
    assert(result == dst);
    assert(strcmp(dst, "Hello") == 0);
    
    /* Test n greater than source length */
    strcpy(dst, "Start");
    result = strncat(dst, "End", 10);
    assert(result == dst);
    assert(strcmp(dst, "StartEnd") == 0);
    
    /* Test truncating in middle of string */
    strcpy(dst, "Part1");
    result = strncat(dst, "Part2Part3Part4", 5);
    assert(result == dst);
    assert(strcmp(dst, "Part1Part2") == 0);
    assert(strlen(dst) == 10);
    
    /* Test multiple limited concatenations */
    strcpy(dst, "");
    strncat(dst, "ABCDEFGH", 3);
    strncat(dst, "IJKLMNOP", 3);
    strncat(dst, "QRSTUVWX", 3);
    assert(strcmp(dst, "ABCIJKQRS") == 0);
    
    /* Test building string with limits */
    strcpy(dst, "Max10:");
    char numbers[] = "0123456789ABCDEF";
    strncat(dst, numbers, 10);
    assert(strcmp(dst, "Max10:0123456789") == 0);
    
    /* Test with single character limit */
    strcpy(dst, "Char");
    strncat(dst, "acter", 1);
    assert(strcmp(dst, "Chara") == 0);
    
    /* Test preserving null terminator */
    memset(dst, 0xFF, BUFFER_SIZE);
    strcpy(dst, "Test");
    strncat(dst, "123", 3);
    assert(dst[7] == '\0');
    assert((unsigned char)dst[8] == 0xFF);
    
    /* Test concatenating from middle of source */
    strcpy(dst, "Get");
    const char *source = "Skip Middle Part";
    strncat(dst, source + 5, 6);
    assert(strcmp(dst, "GetMiddle") == 0);
    
    /* Test with all same characters */
    strcpy(dst, "AAA");
    strncat(dst, "AAAAAA", 3);
    assert(strcmp(dst, "AAAAAA") == 0);
    
    /* Test special characters with limit */
    strcpy(dst, "Lines");
    strncat(dst, "\n\t\r\n\t", 3);
    assert(strcmp(dst, "Lines\n\t\r") == 0);
    
    TEST_PASSED("strncat()");
}

/* Test edge cases */
void test_concat_edge_cases(void) {
    char dst[BUFFER_SIZE];
    char src[BUFFER_SIZE];
    size_t i;
    
    TEST_START("string concatenation edge cases");
    
    /* Test concatenating to nearly full buffer */
    memset(dst, 'A', BUFFER_SIZE - 10);
    dst[BUFFER_SIZE - 10] = '\0';
    
    strcat(dst, "12345678");
    assert(strlen(dst) == BUFFER_SIZE - 2);
    assert(dst[BUFFER_SIZE - 2] == '8');
    assert(dst[BUFFER_SIZE - 1] == '\0');
    
    /* Test strncat with exact remaining space */
    memset(dst, 'B', 100);
    dst[100] = '\0';
    
    strncat(dst, "1234567890", 5);
    assert(strlen(dst) == 105);
    assert(strcmp(dst + 100, "12345") == 0);
    
    /* Test alternating concatenations */
    strcpy(dst, "");
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            strcat(dst, "A");
        } else {
            strcat(dst, "B");
        }
    }
    assert(strcmp(dst, "ABABABABAB") == 0);
    
    /* Test high ASCII values */
    strcpy(dst, "High:");
    unsigned char high[5];
    for (i = 0; i < 4; i++) {
        high[i] = 250 + i;
    }
    high[4] = '\0';
    strcat(dst, (char *)high);
    assert(memcmp(dst + 5, high, 4) == 0);
    
    /* Test strncat with various n values */
    strcpy(src, "Source String");
    
    for (i = 0; i <= strlen(src) + 5; i++) {
        strcpy(dst, "Prefix");
        strncat(dst, src, i);
        
        if (i >= strlen(src)) {
            assert(strcmp(dst, "PrefixSource String") == 0);
        } else {
            assert(strlen(dst) == 6 + i);
            assert(memcmp(dst + 6, src, i) == 0);
            assert(dst[6 + i] == '\0');
        }
    }
    
    /* Test concatenating max length strings */
    memset(src, 'X', 500);
    src[500] = '\0';
    memset(dst, 'Y', 500);
    dst[500] = '\0';
    
    strcat(dst, src);
    assert(strlen(dst) == 1000);
    for (i = 0; i < 500; i++) {
        assert(dst[i] == 'Y');
        assert(dst[i + 500] == 'X');
    }
    
    TEST_PASSED("Edge case");
}

/* Test common usage patterns */
void test_concat_patterns(void) {
    char buffer[256];
    char temp[256];
    
    TEST_START("string concatenation patterns");
    
    /* Pattern 1: Building formatted strings */
    strcpy(buffer, "Error: ");
    strcat(buffer, "File not found");
    strcat(buffer, " (");
    strcat(buffer, "test.txt");
    strcat(buffer, ")");
    assert(strcmp(buffer, "Error: File not found (test.txt)") == 0);
    
    /* Pattern 2: Safe concatenation with length check */
    strcpy(buffer, "Start");
    const char *append = "Very long string that might overflow";
    
    size_t space_left = sizeof(buffer) - strlen(buffer) - 1;
    strncat(buffer, append, space_left);
    assert(strlen(buffer) < sizeof(buffer));
    
    /* Pattern 3: Building CSV line */
    strcpy(buffer, "");
    const char *fields[] = {"Name", "Age", "City"};
    size_t num_fields = 3;
    size_t j;
    
    for (j = 0; j < num_fields; j++) {
        if (j > 0) strcat(buffer, ",");
        strcat(buffer, fields[j]);
    }
    assert(strcmp(buffer, "Name,Age,City") == 0);
    
    /* Pattern 4: Building file path */
    const char *dir = "/home/user";
    const char *file = "document.txt";
    
    strcpy(buffer, dir);
    if (buffer[strlen(buffer) - 1] != '/') {
        strcat(buffer, "/");
    }
    strcat(buffer, file);
    assert(strcmp(buffer, "/home/user/document.txt") == 0);
    
    /* Pattern 5: Appending with delimiter */
    strcpy(buffer, "Item1");
    const char *items[] = {"Item2", "Item3", "Item4"};
    
    for (j = 0; j < 3; j++) {
        strcat(buffer, "; ");
        strcat(buffer, items[j]);
    }
    assert(strcmp(buffer, "Item1; Item2; Item3; Item4") == 0);
    
    /* Pattern 6: Building query string */
    strcpy(buffer, "?");
    strcat(buffer, "name=");
    strcat(buffer, "John");
    strcat(buffer, "&");
    strcat(buffer, "age=");
    strcat(buffer, "30");
    assert(strcmp(buffer, "?name=John&age=30") == 0);
    
    /* Pattern 7: Limited append for display */
    strcpy(buffer, "Display: ");
    const char *long_text = "This is a very long text that needs to be truncated for display purposes";
    strncat(buffer, long_text, 20);
    strcat(buffer, "...");
    assert(strcmp(buffer, "Display: This is a very long ...") == 0);
    
    /* Pattern 8: Building command line */
    strcpy(buffer, "gcc");
    const char *flags[] = {" -Wall", " -O2", " -o output", " input.c"};
    
    for (j = 0; j < 4; j++) {
        strcat(buffer, flags[j]);
    }
    assert(strcmp(buffer, "gcc -Wall -O2 -o output input.c") == 0);
    
    TEST_PASSED("Pattern");
}

/* Test Unicode concatenation */
void test_unicode_concat(void) {
    char dst[BUFFER_SIZE];
    
    TEST_START("Unicode string concatenation");
    
    /* Test UTF-8 concatenation */
    strcpy(dst, "Hello");
    strcat(dst, " ");
    strcat(dst, "世界");
    strcat(dst, " ");
    strcat(dst, "🌍");
    assert(strcmp(dst, "Hello 世界 🌍") == 0);
    
    /* Test strncat with UTF-8 (careful with boundaries) */
    strcpy(dst, "Start");
    /* "你好" - each character is 3 bytes */
    strncat(dst, "你好世界", 6); /* Only first 2 characters */
    assert(strlen(dst) == 5 + 6);
    
    /* Test mixed scripts */
    strcpy(dst, "Mix: ");
    strcat(dst, "ABC");
    strcat(dst, "あいう");
    strcat(dst, "123");
    strcat(dst, "가나다");
    
    /* Test emoji concatenation */
    strcpy(dst, "Happy");
    strcat(dst, " 😀 ");
    strcat(dst, "Birthday");
    strcat(dst, " 🎂");
    
    TEST_PASSED("Unicode concatenation");
}

/* Test performance patterns */
void test_concat_performance(void) {
    char buffer[BUFFER_SIZE];
    char small[10];
    size_t i;
    
    TEST_START("concatenation performance patterns");
    
    /* Pattern 1: Many small concatenations */
    strcpy(buffer, "");
    for (i = 0; i < 100; i++) {
        strcat(buffer, "X");
    }
    assert(strlen(buffer) == 100);
    
    /* Pattern 2: Building with sprintf alternative */
    strcpy(buffer, "");
    for (i = 0; i < 10; i++) {
        sprintf(small, "%zu", i);
        if (i > 0) strcat(buffer, ",");
        strcat(buffer, small);
    }
    assert(strcmp(buffer, "0,1,2,3,4,5,6,7,8,9") == 0);
    
    /* Pattern 3: Prepending (inefficient but sometimes needed) */
    strcpy(buffer, "End");
    strcpy(small, "Start");
    
    /* Prepend by copying */
    char temp[BUFFER_SIZE];
    strcpy(temp, small);
    strcat(temp, buffer);
    strcpy(buffer, temp);
    assert(strcmp(buffer, "StartEnd") == 0);
    
    TEST_PASSED("Performance pattern");
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 string.h String Concatenation Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_strcat();
    test_strncat();
    test_concat_edge_cases();
    test_concat_patterns();
    test_unicode_concat();
    test_concat_performance();
    
    TEST_SUITE_PASSED("string concatenation");
}
