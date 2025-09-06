/*
 * test_string_copy.c - PSE51 string.h string copy test suite
 * Tests: strcpy, strncpy
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for string copy functions
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "../test_colors.h"
#define BUFFER_SIZE 1024

/* Test strcpy() function */
void test_strcpy(void) {
    char src[BUFFER_SIZE];
    char dst[BUFFER_SIZE];
    char *result;
    size_t i;
    
    TEST_START("strcpy()");
    
    /* Test basic copy */
    strcpy(src, "Hello, World!");
    memset(dst, 0xFF, BUFFER_SIZE);
    
    result = strcpy(dst, src);
    assert(result == dst);
    assert(strcmp(dst, "Hello, World!") == 0);
    assert(strlen(dst) == strlen(src));
    
    /* Verify null terminator is copied */
    assert(dst[13] == '\0');
    
    /* Verify nothing beyond null terminator is modified */
    assert((unsigned char)dst[14] == 0xFF);
    
    /* Test empty string */
    result = strcpy(dst, "");
    assert(result == dst);
    assert(dst[0] == '\0');
    assert(strlen(dst) == 0);
    
    /* Test single character */
    result = strcpy(dst, "A");
    assert(strcmp(dst, "A") == 0);
    assert(dst[1] == '\0');
    
    /* Test long string */
    char long_str[BUFFER_SIZE];
    for (i = 0; i < BUFFER_SIZE - 2; i++) {
        long_str[i] = 'A' + (i % 26);
    }
    long_str[BUFFER_SIZE - 2] = '\0';
    
    result = strcpy(dst, long_str);
    assert(strcmp(dst, long_str) == 0);
    assert(strlen(dst) == BUFFER_SIZE - 2);
    
    /* Test special characters */
    result = strcpy(dst, "Line 1\nLine 2\tTabbed\r\nCRLF");
    assert(strcmp(dst, "Line 1\nLine 2\tTabbed\r\nCRLF") == 0);
    
    /* Test with null characters in source (stops at first null) */
    char null_str[] = {'H', 'e', 'l', 'l', 'o', '\0', 'W', 'o', 'r', 'l', 'd', '\0'};
    result = strcpy(dst, null_str);
    assert(strcmp(dst, "Hello") == 0);
    assert(strlen(dst) == 5);
    
    /* Test all ASCII printable characters */
    char ascii_str[96];
    for (i = 0; i < 95; i++) {
        ascii_str[i] = ' ' + i; /* Space to tilde */
    }
    ascii_str[95] = '\0';
    
    result = strcpy(dst, ascii_str);
    assert(strcmp(dst, ascii_str) == 0);
    
    /* Test copying to same buffer (different locations) */
    strcpy(dst, "Original string");
    strcpy(dst + 20, "Another string");
    assert(strcmp(dst, "Original string") == 0);
    assert(strcmp(dst + 20, "Another string") == 0);
    
    /* Test UTF-8 strings (if supported) */
    result = strcpy(dst, "Hello, 世界! 🌍");
    assert(strcmp(dst, "Hello, 世界! 🌍") == 0);
    
    TEST_PASSED("strcpy()");
}

/* Test strncpy() function */
void test_strncpy(void) {
    char src[BUFFER_SIZE];
    char dst[BUFFER_SIZE];
    char *result;
    size_t i;
    
    TEST_START("strncpy()");
    
    /* Test basic copy with exact length */
    strcpy(src, "Hello");
    memset(dst, 0xFF, BUFFER_SIZE);
    
    result = strncpy(dst, src, 5);
    assert(result == dst);
    assert(memcmp(dst, "Hello", 5) == 0);
    assert((unsigned char)dst[5] == 0xFF); /* Not null terminated */
    
    /* Test copy with null termination */
    memset(dst, 0xFF, BUFFER_SIZE);
    result = strncpy(dst, src, 6);
    assert(result == dst);
    assert(strcmp(dst, "Hello") == 0);
    assert(dst[5] == '\0');
    assert((unsigned char)dst[6] == 0xFF);
    
    /* Test copy with padding */
    memset(dst, 0xFF, BUFFER_SIZE);
    result = strncpy(dst, src, 10);
    assert(result == dst);
    assert(strcmp(dst, "Hello") == 0);
    
    /* Verify padding with nulls */
    for (i = 5; i < 10; i++) {
        assert(dst[i] == '\0');
    }
    assert((unsigned char)dst[10] == 0xFF);
    
    /* Test truncation */
    strcpy(src, "This is a very long string");
    memset(dst, 0xFF, BUFFER_SIZE);
    
    result = strncpy(dst, src, 10);
    assert(result == dst);
    assert(memcmp(dst, "This is a ", 10) == 0);
    assert((unsigned char)dst[10] == 0xFF); /* No null terminator */
    
    /* Test empty string */
    memset(dst, 0xFF, BUFFER_SIZE);
    result = strncpy(dst, "", 5);
    assert(result == dst);
    
    /* Should pad with nulls */
    for (i = 0; i < 5; i++) {
        assert(dst[i] == '\0');
    }
    assert((unsigned char)dst[5] == 0xFF);
    
    /* Test n = 0 */
    memset(dst, 0xFF, BUFFER_SIZE);
    result = strncpy(dst, "Hello", 0);
    assert(result == dst);
    assert((unsigned char)dst[0] == 0xFF); /* Nothing copied */
    
    /* Test single character with padding */
    memset(dst, 0xFF, BUFFER_SIZE);
    result = strncpy(dst, "A", 5);
    assert(dst[0] == 'A');
    for (i = 1; i < 5; i++) {
        assert(dst[i] == '\0');
    }
    
    /* Test copying full buffer */
    for (i = 0; i < BUFFER_SIZE - 1; i++) {
        src[i] = 'A' + (i % 26);
    }
    src[BUFFER_SIZE - 1] = '\0';
    
    memset(dst, 0, BUFFER_SIZE);
    result = strncpy(dst, src, BUFFER_SIZE - 1);
    assert(memcmp(dst, src, BUFFER_SIZE - 1) == 0);
    
    /* Test special characters with truncation */
    strcpy(src, "Line1\nLine2\tTab");
    memset(dst, 0xFF, BUFFER_SIZE);
    
    result = strncpy(dst, src, 8);
    assert(memcmp(dst, "Line1\nLi", 8) == 0);
    assert((unsigned char)dst[8] == 0xFF);
    
    /* Test copying to overlapping regions (undefined behavior) */
    /* Don't test this as it's undefined */
    
    /* Test pattern: safe string copy */
    char safe_dst[20];
    const char *long_src = "This string is too long for the buffer";
    
    memset(safe_dst, 0xFF, sizeof(safe_dst));
    strncpy(safe_dst, long_src, sizeof(safe_dst) - 1);
    safe_dst[sizeof(safe_dst) - 1] = '\0'; /* Ensure null termination */
    
    assert(strlen(safe_dst) == sizeof(safe_dst) - 1);
    assert(memcmp(safe_dst, long_src, sizeof(safe_dst) - 1) == 0);
    
    /* Test all nulls source */
    char null_src[10] = {0};
    memset(dst, 0xFF, BUFFER_SIZE);
    
    result = strncpy(dst, null_src, 10);
    for (i = 0; i < 10; i++) {
        assert(dst[i] == '\0');
    }
    
    TEST_PASSED("strncpy()");
}

/* Test edge cases */
void test_copy_edge_cases(void) {
    char dst[BUFFER_SIZE];
    char src[BUFFER_SIZE];
    size_t i;
    
    TEST_START("string copy edge cases");
    
    /* Test maximum length strings */
    memset(src, 'X', BUFFER_SIZE - 1);
    src[BUFFER_SIZE - 1] = '\0';
    
    strcpy(dst, src);
    assert(strlen(dst) == BUFFER_SIZE - 1);
    assert(strcmp(dst, src) == 0);
    
    /* Test strncpy with exact buffer size */
    memset(dst, 0, BUFFER_SIZE);
    strncpy(dst, src, BUFFER_SIZE);
    assert(memcmp(dst, src, BUFFER_SIZE) == 0);
    
    /* Test alternating characters */
    for (i = 0; i < BUFFER_SIZE - 1; i++) {
        src[i] = (i % 2) ? 'A' : 'B';
    }
    src[BUFFER_SIZE - 1] = '\0';
    
    strcpy(dst, src);
    assert(strcmp(dst, src) == 0);
    
    /* Test high ASCII values */
    for (i = 0; i < 255; i++) {
        src[i] = i + 1; /* 1 to 255, avoiding null */
    }
    src[255] = '\0';
    
    strcpy(dst, src);
    assert(strcmp(dst, src) == 0);
    assert(strlen(dst) == 255);
    
    /* Test strncpy with various n values */
    strcpy(src, "Test string");
    
    for (i = 0; i <= strlen(src) + 5; i++) {
        memset(dst, 0xFF, BUFFER_SIZE);
        strncpy(dst, src, i);
        
        if (i <= strlen(src)) {
            assert(memcmp(dst, src, i) == 0);
        } else {
            assert(strcmp(dst, src) == 0);
            /* Check padding */
            size_t j;
            for (j = strlen(src); j < i; j++) {
                assert(dst[j] == '\0');
            }
        }
    }
    
    TEST_PASSED("Edge case");
}

/* Test common usage patterns */
void test_copy_patterns(void) {
    char buffer[256];
    char temp[256];
    
    TEST_START("string copy patterns");
    
    /* Pattern 1: Safe string copy with guaranteed null termination */
    const char *input = "User provided input that might be too long";
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    assert(strlen(buffer) <= sizeof(buffer) - 1);
    
    /* Pattern 2: Copying with length check */
    const char *src = "Source string";
    if (strlen(src) < sizeof(buffer)) {
        strcpy(buffer, src);
        assert(strcmp(buffer, src) == 0);
    }
    
    /* Pattern 3: Building strings */
    strcpy(buffer, "Hello");
    strcpy(buffer + strlen(buffer), ", ");
    strcpy(buffer + strlen(buffer), "World!");
    assert(strcmp(buffer, "Hello, World!") == 0);
    
    /* Pattern 4: Copying struct fields */
    struct {
        char name[32];
        char description[128];
    } item1, item2;
    
    strcpy(item1.name, "Test Item");
    strcpy(item1.description, "This is a test item");
    
    strcpy(item2.name, item1.name);
    strcpy(item2.description, item1.description);
    
    assert(strcmp(item2.name, item1.name) == 0);
    assert(strcmp(item2.description, item1.description) == 0);
    
    /* Pattern 5: Tokenizing with copy */
    const char *full_path = "/home/user/file.txt";
    char *slash = strrchr(full_path, '/');
    if (slash) {
        strcpy(buffer, slash + 1);
        assert(strcmp(buffer, "file.txt") == 0);
    }
    
    /* Pattern 6: Padding sensitive data */
    strcpy(buffer, "password123");
    size_t len = strlen(buffer);
    
    /* Clear password, keeping same length */
    strncpy(buffer, "***********", len);
    buffer[len] = '\0';
    assert(strlen(buffer) == len);
    assert(strchr(buffer, 'p') == NULL);
    
    /* Pattern 7: Copying with transformation */
    const char *original = "HELLO WORLD";
    size_t i;
    
    /* Manual lowercase while copying */
    for (i = 0; original[i]; i++) {
        buffer[i] = (original[i] >= 'A' && original[i] <= 'Z') 
                    ? original[i] + 32 : original[i];
    }
    buffer[i] = '\0';
    assert(strcmp(buffer, "hello world") == 0);
    
    TEST_PASSED("Pattern");
}

/* Test Unicode and multibyte strings */
void test_unicode_copy(void) {
    char src[BUFFER_SIZE];
    char dst[BUFFER_SIZE];
    
    TEST_START("Unicode string copy");
    
    /* Test UTF-8 strings */
    strcpy(src, "Hello, 世界!");
    strcpy(dst, src);
    assert(strcmp(dst, src) == 0);
    
    /* Test with emojis */
    strcpy(src, "Test 😀 🎉 🌟");
    strcpy(dst, src);
    assert(strcmp(dst, src) == 0);
    
    /* Test strncpy with UTF-8 (careful about character boundaries) */
    strcpy(src, "你好世界");
    
    /* Each Chinese character is 3 bytes in UTF-8 */
    memset(dst, 0, BUFFER_SIZE);
    strncpy(dst, src, 6); /* Copy first 2 characters */
    dst[6] = '\0';
    
    /* We copied exactly 2 complete characters */
    assert(strlen(dst) == 6);
    
    /* Test mixed ASCII and Unicode */
    strcpy(src, "ABC你好DEF");
    strcpy(dst, src);
    assert(strcmp(dst, src) == 0);
    
    TEST_PASSED("Unicode copy");
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 string.h String Copy Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_strcpy();
    test_strncpy();
    test_copy_edge_cases();
    test_copy_patterns();
    test_unicode_copy();
    
    TEST_SUITE_PASSED("string copy");
}
