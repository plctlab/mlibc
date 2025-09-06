/*
 * test_string_search.c - PSE51 string.h string search test suite
 * Tests: strchr, strrchr, strstr, strpbrk, strcspn, strspn, strlen
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for string search functions
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <limits.h>

#include "../test_colors.h"
#define BUFFER_SIZE 1024

/* Test strlen() function */
void test_strlen(void) {
    size_t len;
    char buffer[BUFFER_SIZE];
    size_t i;
    
    TEST_START("strlen()");
    
    /* Test empty string */
    len = strlen("");
    assert(len == 0);
    
    /* Test single character */
    len = strlen("A");
    assert(len == 1);
    
    /* Test normal string */
    len = strlen("Hello, World!");
    assert(len == 13);
    
    /* Test string with special characters */
    len = strlen("Line1\nLine2\tTab\0Hidden");
    assert(len == 15); /* Stops at first null */
    
    /* Test long string */
    for (i = 0; i < BUFFER_SIZE - 1; i++) {
        buffer[i] = 'A' + (i % 26);
    }
    buffer[BUFFER_SIZE - 1] = '\0';
    
    len = strlen(buffer);
    assert(len == BUFFER_SIZE - 1);
    
    /* Test all ASCII printable */
    for (i = 0; i < 95; i++) {
        buffer[i] = ' ' + i;
    }
    buffer[95] = '\0';
    
    len = strlen(buffer);
    assert(len == 95);
    
    /* Test UTF-8 string (byte length, not character count) */
    len = strlen("Hello, 世界!");
    assert(len > 8); /* More bytes than visible characters */
    
    /* Test string with only spaces */
    len = strlen("     ");
    assert(len == 5);
    
    /* Test numeric string */
    len = strlen("1234567890");
    assert(len == 10);
    
    /* Test very long string */
    char *long_str = (char *)malloc(100000);
    if (long_str) {
        memset(long_str, 'X', 99999);
        long_str[99999] = '\0';
        len = strlen(long_str);
        assert(len == 99999);
        free(long_str);
    }
    
    TEST_PASSED("strlen()");
}

/* Test strchr() function */
void test_strchr(void) {
    const char *str;
    char *result;
    char buffer[BUFFER_SIZE];
    size_t i;
    
    TEST_START("strchr()");
    
    /* Test finding character */
    str = "Hello, World!";
    result = strchr(str, 'W');
    assert(result != NULL);
    assert(result == str + 7);
    assert(*result == 'W');
    
    /* Test finding first occurrence */
    str = "abcabcabc";
    result = strchr(str, 'b');
    assert(result == str + 1);
    
    /* Test not finding character */
    result = strchr(str, 'x');
    assert(result == NULL);
    
    /* Test finding null terminator */
    str = "Test";
    result = strchr(str, '\0');
    assert(result != NULL);
    assert(result == str + 4);
    assert(*result == '\0');
    
    /* Test finding at beginning */
    str = "Test";
    result = strchr(str, 'T');
    assert(result == str);
    
    /* Test finding at end */
    str = "Test";
    result = strchr(str, 't');
    assert(result == str + 3);
    
    /* Test empty string */
    str = "";
    result = strchr(str, 'a');
    assert(result == NULL);
    
    result = strchr(str, '\0');
    assert(result == str);
    
    /* Test special characters */
    str = "Line1\nLine2\tTab";
    result = strchr(str, '\n');
    assert(result == str + 5);
    
    result = strchr(str, '\t');
    assert(result == str + 11);
    
    /* Test all ASCII */
    for (i = 1; i < 128; i++) {
        buffer[i-1] = i;
    }
    buffer[127] = '\0';
    
    for (i = 1; i < 128; i++) {
        result = strchr(buffer, i);
        assert(result == buffer + i - 1);
    }
    
    /* Test high ASCII values */
    buffer[0] = (char)200;
    buffer[1] = (char)255;
    buffer[2] = '\0';
    
    result = strchr(buffer, 200);
    assert(result == buffer);
    
    result = strchr(buffer, 255);
    assert(result == buffer + 1);
    
    /* Test case sensitivity */
    str = "Hello";
    result = strchr(str, 'h');
    assert(result == NULL); /* Case sensitive */
    
    TEST_PASSED("strchr()");
}

/* Test strrchr() function */
void test_strrchr(void) {
    const char *str;
    char *result;
    
    TEST_START("strrchr()");
    
    /* Test finding last occurrence */
    str = "abcabcabc";
    result = strrchr(str, 'b');
    assert(result == str + 7); /* Last 'b' */
    
    /* Test single occurrence */
    str = "Hello, World!";
    result = strrchr(str, 'W');
    assert(result == str + 7);
    
    /* Test not finding character */
    result = strrchr(str, 'x');
    assert(result == NULL);
    
    /* Test finding null terminator */
    str = "Test";
    result = strrchr(str, '\0');
    assert(result == str + 4);
    
    /* Test character at beginning only */
    str = "Test";
    result = strrchr(str, 'T');
    assert(result == str);
    
    /* Test character at end only */
    str = "Test";
    result = strrchr(str, 't');
    assert(result == str + 3);
    
    /* Test empty string */
    str = "";
    result = strrchr(str, 'a');
    assert(result == NULL);
    
    result = strrchr(str, '\0');
    assert(result == str);
    
    /* Test path-like string */
    str = "/home/user/file.txt";
    result = strrchr(str, '/');
    assert(result == str + 10); /* Last slash */
    
    /* Test extension finding */
    str = "document.backup.txt";
    result = strrchr(str, '.');
    assert(result == str + 15); /* Last dot */
    assert(strcmp(result, ".txt") == 0);
    
    /* Test all same character */
    str = "aaaaaaa";
    result = strrchr(str, 'a');
    assert(result == str + 6); /* Last one */
    
    /* Test special characters */
    str = "Line1\nLine2\nLine3";
    result = strrchr(str, '\n');
    assert(result == str + 11); /* Last newline */
    
    TEST_PASSED("strrchr()");
}

/* Test strstr() function */
void test_strstr(void) {
    const char *haystack;
    char *result;
    char buffer[BUFFER_SIZE];
    
    TEST_START("strstr()");
    
    /* Test finding substring */
    haystack = "Hello, World!";
    result = strstr(haystack, "World");
    assert(result != NULL);
    assert(result == haystack + 7);
    assert(strncmp(result, "World!", 6) == 0);
    
    /* Test finding at beginning */
    result = strstr(haystack, "Hello");
    assert(result == haystack);
    
    /* Test finding at end */
    haystack = "Testing end";
    result = strstr(haystack, "end");
    assert(result == haystack + 8);
    
    /* Test not finding substring */
    result = strstr(haystack, "xyz");
    assert(result == NULL);
    
    /* Test empty needle */
    result = strstr(haystack, "");
    assert(result == haystack); /* Should return haystack */
    
    /* Test empty haystack */
    result = strstr("", "test");
    assert(result == NULL);
    
    /* Test both empty */
    result = strstr("", "");
    assert(result != NULL); /* Should return empty string */
    
    /* Test single character needle */
    haystack = "abcdef";
    result = strstr(haystack, "d");
    assert(result == haystack + 3);
    
    /* Test overlapping occurrences */
    haystack = "aaabaaabaaab";
    result = strstr(haystack, "aaab");
    assert(result == haystack); /* First occurrence */
    
    /* Test case sensitivity */
    haystack = "Hello World";
    result = strstr(haystack, "world");
    assert(result == NULL); /* Case sensitive */
    
    /* Test finding repeated pattern */
    haystack = "abcabcabcabc";
    result = strstr(haystack, "abcabc");
    assert(result == haystack); /* First occurrence */
    
    /* Test special characters */
    haystack = "Line1\nLine2\tTab";
    result = strstr(haystack, "\nLine2");
    assert(result == haystack + 5);
    
    /* Test finding itself */
    haystack = "complete string";
    result = strstr(haystack, haystack);
    assert(result == haystack);
    
    /* Test long needle */
    strcpy(buffer, "Start");
    strcat(buffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    strcat(buffer, "End");
    
    result = strstr(buffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    assert(result == buffer + 5);
    
    /* Test needle longer than haystack */
    result = strstr("short", "much longer needle");
    assert(result == NULL);
    
    TEST_PASSED("strstr()");
}

/* Test strpbrk() function */
void test_strpbrk(void) {
    const char *str;
    char *result;
    
    TEST_START("strpbrk()");
    
    /* Test finding one of several characters */
    str = "Hello, World!";
    result = strpbrk(str, "aeiou");
    assert(result == str + 1); /* First vowel 'e' */
    
    /* Test finding punctuation */
    result = strpbrk(str, ",.!?");
    assert(result == str + 5); /* Comma */
    
    /* Test not finding any */
    result = strpbrk(str, "xyz");
    assert(result == NULL);
    
    /* Test empty accept string */
    result = strpbrk(str, "");
    assert(result == NULL);
    
    /* Test empty source string */
    result = strpbrk("", "abc");
    assert(result == NULL);
    
    /* Test finding first of many */
    str = "abcdefg";
    result = strpbrk(str, "gfedcba");
    assert(result == str); /* First match is 'a' */
    
    /* Test special characters */
    str = "Normal\nText\tWith\rSpecial";
    result = strpbrk(str, "\n\r\t");
    assert(result == str + 6); /* First special is '\n' */
    
    /* Test digits */
    str = "Version 2.5.1";
    result = strpbrk(str, "0123456789");
    assert(result == str + 8); /* First digit '2' */
    
    /* Test all same in accept */
    str = "abcabc";
    result = strpbrk(str, "aaa");
    assert(result == str); /* First 'a' */
    
    /* Test finding null (should not) */
    str = "Test";
    result = strpbrk(str, "t\0x"); /* Null in accept string */
    assert(result == str + 3); /* Finds 't' not null */
    
    /* Test whitespace */
    str = "No space at start";
    result = strpbrk(str, " \t\n");
    assert(result == str + 2); /* First space */
    
    /* Test finding from large set */
    str = "Find vowel";
    result = strpbrk(str, "AEIOUaeiou");
    assert(result == str + 1); /* 'i' in Find */
    
    TEST_PASSED("strpbrk()");
}

/* Test strcspn() function */
void test_strcspn(void) {
    size_t len;
    const char *str;
    
    TEST_START("strcspn()");
    
    /* Test basic span */
    str = "Hello, World!";
    len = strcspn(str, ",.!?");
    assert(len == 5); /* Stops at comma */
    
    /* Test no matching characters */
    len = strcspn(str, "xyz");
    assert(len == 13); /* Full string length */
    
    /* Test empty reject string */
    len = strcspn(str, "");
    assert(len == 13); /* Full string length */
    
    /* Test empty source string */
    len = strcspn("", "abc");
    assert(len == 0);
    
    /* Test matching first character */
    str = "abcdef";
    len = strcspn(str, "a");
    assert(len == 0);
    
    /* Test digits */
    str = "abc123def";
    len = strcspn(str, "0123456789");
    assert(len == 3); /* Stops at '1' */
    
    /* Test whitespace */
    str = "NoSpaceHere End";
    len = strcspn(str, " \t\n");
    assert(len == 11); /* Stops at space */
    
    /* Test special characters */
    str = "Normal\nText";
    len = strcspn(str, "\n\r\t");
    assert(len == 6); /* Stops at newline */
    
    /* Test identical strings */
    str = "test";
    len = strcspn(str, "test");
    assert(len == 0); /* First char is in reject */
    
    /* Test vowels */
    str = "bcdfghjklmnp";
    len = strcspn(str, "aeiou");
    assert(len == 12); /* No vowels */
    
    /* Test with null in reject (should be ignored) */
    str = "Testing";
    len = strcspn(str, "x\0y"); /* Null terminates reject */
    assert(len == 7); /* No 'x' found */
    
    TEST_PASSED("strcspn()");
}

/* Test strspn() function */
void test_strspn(void) {
    size_t len;
    const char *str;
    
    TEST_START("strspn()");
    
    /* Test basic span */
    str = "123abc456";
    len = strspn(str, "0123456789");
    assert(len == 3); /* First 3 are digits */
    
    /* Test all matching */
    str = "12345";
    len = strspn(str, "0123456789");
    assert(len == 5); /* All are digits */
    
    /* Test none matching */
    str = "abcdef";
    len = strspn(str, "0123456789");
    assert(len == 0); /* No digits */
    
    /* Test empty accept string */
    len = strspn(str, "");
    assert(len == 0);
    
    /* Test empty source string */
    len = strspn("", "abc");
    assert(len == 0);
    
    /* Test whitespace prefix */
    str = "   \t\nText";
    len = strspn(str, " \t\n");
    assert(len == 5); /* All whitespace chars */
    
    /* Test identifier characters */
    str = "var_name123!@#";
    len = strspn(str, "abcdefghijklmnopqrstuvwxyz_0123456789");
    assert(len == 11); /* Stops at '!' */
    
    /* Test hexadecimal */
    str = "0xDEADBEEF";
    len = strspn(str + 2, "0123456789ABCDEFabcdef");
    assert(len == 8); /* All hex digits after 0x */
    
    /* Test identical strings */
    str = "test";
    len = strspn(str, "test");
    assert(len == 4); /* All match */
    
    /* Test repeated characters */
    str = "aaabbbccc";
    len = strspn(str, "a");
    assert(len == 3); /* First 3 a's */
    
    /* Test with larger accept set */
    str = "abc123!@#";
    len = strspn(str, "abcdefghijklmnopqrstuvwxyz0123456789");
    assert(len == 6); /* Letters and digits */
    
    TEST_PASSED("strspn()");
}

/* Test edge cases */
void test_search_edge_cases(void) {
    char buffer[BUFFER_SIZE];
    char *result;
    size_t len;
    size_t i;
    
    TEST_START("string search edge cases");
    
    /* Test maximum length strings */
    memset(buffer, 'A', BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';
    
    len = strlen(buffer);
    assert(len == BUFFER_SIZE - 1);
    
    /* strchr on long string */
    buffer[BUFFER_SIZE - 2] = 'B';
    result = strchr(buffer, 'B');
    assert(result == buffer + BUFFER_SIZE - 2);
    
    /* strrchr on long string */
    buffer[0] = 'B';
    result = strrchr(buffer, 'B');
    assert(result == buffer + BUFFER_SIZE - 2); /* Last one */
    
    /* Test all byte values */
    for (i = 1; i < 256; i++) {
        buffer[i-1] = i;
    }
    buffer[255] = '\0';
    
    /* Find each byte */
    for (i = 1; i < 256; i++) {
        result = strchr(buffer, i);
        if (i < 256) {
            assert(result == buffer + i - 1);
        }
    }
    
    /* Test strstr with patterns at boundaries */
    strcpy(buffer, "abcdefghijklmnopqrstuvwxyz");
    
    /* Pattern at start */
    result = strstr(buffer, "abc");
    assert(result == buffer);
    
    /* Pattern at end */
    result = strstr(buffer, "xyz");
    assert(result == buffer + 23);
    
    /* Pattern spanning middle */
    result = strstr(buffer, "lmnop");
    assert(result == buffer + 11);
    
    TEST_PASSED("Edge case");
}

/* Test common patterns */
void test_search_patterns(void) {
    char buffer[BUFFER_SIZE];
    char *result;
    size_t len;
    
    TEST_START("string search patterns");
    
    /* Pattern 1: Finding file extension */
    const char *filename = "document.backup.txt";
    result = strrchr(filename, '.');
    assert(result != NULL);
    assert(strcmp(result + 1, "txt") == 0);
    
    /* Pattern 2: Parsing paths */
    const char *path = "/home/user/documents/file.txt";
    result = strrchr(path, '/');
    assert(result != NULL);
    assert(strcmp(result + 1, "file.txt") == 0);
    
    /* Pattern 3: Finding delimiters */
    const char *csv = "field1,field2,field3";
    result = strchr(csv, ',');
    assert(result == csv + 6);
    
    /* Pattern 4: Validating input */
    const char *input = "abc123";
    len = strspn(input, "abcdefghijklmnopqrstuvwxyz0123456789");
    assert(len == strlen(input)); /* All valid chars */
    
    /* Pattern 5: Trimming whitespace */
    const char *text = "   Hello   ";
    len = strspn(text, " \t\n");
    assert(len == 3); /* Leading spaces */
    
    /* Pattern 6: Finding first non-digit */
    const char *mixed = "123abc";
    len = strspn(mixed, "0123456789");
    assert(mixed[len] == 'a'); /* First non-digit */
    
    /* Pattern 7: Parsing key=value */
    const char *config = "name=value";
    result = strchr(config, '=');
    assert(result != NULL);
    assert(result == config + 4);
    
    /* Pattern 8: Finding any whitespace */
    const char *line = "Word1 Word2\tWord3\n";
    result = strpbrk(line, " \t\n");
    assert(result == line + 5); /* First space */
    
    /* Pattern 9: Checking for special chars */
    const char *password = "Pass123!";
    result = strpbrk(password, "!@#$%^&*");
    assert(result != NULL); /* Has special char */
    
    /* Pattern 10: Finding substring for replacement */
    strcpy(buffer, "The old text with old words");
    result = strstr(buffer, "old");
    assert(result == buffer + 4); /* First "old" */
    
    TEST_PASSED("Pattern");
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 string.h String Search Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_strlen();
    test_strchr();
    test_strrchr();
    test_strstr();
    test_strpbrk();
    test_strcspn();
    test_strspn();
    test_search_edge_cases();
    test_search_patterns();
    
    TEST_SUITE_PASSED("string search");
}
