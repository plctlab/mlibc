/*
 * test_string_compare.c - PSE51 string.h string comparison test suite
 * Tests: strcmp, strncmp, strcoll
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for string comparison functions
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <limits.h>

#include "../test_colors.h"
#define BUFFER_SIZE 1024

/* Test strcmp() function */
void test_strcmp(void) {
    int result;
    char str1[BUFFER_SIZE];
    char str2[BUFFER_SIZE];
    size_t i;
    
    TEST_START("strcmp()");
    
    /* Test equal strings */
    result = strcmp("Hello", "Hello");
    assert(result == 0);
    
    /* Test different strings - first less */
    result = strcmp("Hello", "World");
    assert(result < 0);
    
    /* Test different strings - first greater */
    result = strcmp("World", "Hello");
    assert(result > 0);
    
    /* Test empty strings */
    result = strcmp("", "");
    assert(result == 0);
    
    /* Test empty vs non-empty */
    result = strcmp("", "Test");
    assert(result < 0);
    
    result = strcmp("Test", "");
    assert(result > 0);
    
    /* Test different lengths - prefix */
    result = strcmp("Test", "Testing");
    assert(result < 0);
    
    result = strcmp("Testing", "Test");
    assert(result > 0);
    
    /* Test single character difference */
    result = strcmp("Test1", "Test2");
    assert(result < 0);
    
    /* Test case sensitivity */
    result = strcmp("hello", "Hello");
    assert(result > 0); /* 'h' > 'H' in ASCII */
    
    result = strcmp("HELLO", "hello");
    assert(result < 0);
    
    /* Test with special characters */
    result = strcmp("Line1\n", "Line1\t");
    assert(result != 0); /* '\n' != '\t' */
    
    /* Test numeric strings */
    result = strcmp("123", "124");
    assert(result < 0);
    
    result = strcmp("9", "10");
    assert(result > 0); /* '9' > '1' lexicographically */
    
    /* Test high ASCII values */
    strcpy(str1, "Test\x80");
    strcpy(str2, "Test\x7F");
    result = strcmp(str1, str2);
    assert(result > 0); /* 0x80 > 0x7F */
    
    /* Test all same except last character */
    memset(str1, 'A', 100);
    memset(str2, 'A', 100);
    str1[100] = 'B';
    str2[100] = 'C';
    str1[101] = '\0';
    str2[101] = '\0';
    
    result = strcmp(str1, str2);
    assert(result < 0);
    
    /* Test comparing with self */
    strcpy(str1, "Self compare");
    result = strcmp(str1, str1);
    assert(result == 0);
    
    /* Test long identical strings */
    for (i = 0; i < BUFFER_SIZE - 1; i++) {
        str1[i] = str2[i] = 'A' + (i % 26);
    }
    str1[BUFFER_SIZE - 1] = str2[BUFFER_SIZE - 1] = '\0';
    
    result = strcmp(str1, str2);
    assert(result == 0);
    
    /* Test UTF-8 strings (byte-wise comparison) */
    result = strcmp("café", "cafe");
    assert(result != 0); /* Different due to accent */
    
    TEST_PASSED("strcmp()");
}

/* Test strncmp() function */
void test_strncmp(void) {
    int result;
    size_t i;
    
    TEST_START("strncmp()");
    
    /* Test equal strings with exact length */
    result = strncmp("Hello", "Hello", 5);
    assert(result == 0);
    
    /* Test equal strings with extra length */
    result = strncmp("Hello", "Hello", 10);
    assert(result == 0);
    
    /* Test different strings with limited length */
    result = strncmp("Hello", "Help", 3);
    assert(result == 0); /* First 3 chars are same */
    
    result = strncmp("Hello", "Help", 4);
    assert(result < 0); /* 'l' < 'p' */
    
    /* Test n = 0 */
    result = strncmp("ABC", "XYZ", 0);
    assert(result == 0); /* Always equal when n=0 */
    
    /* Test empty strings */
    result = strncmp("", "", 5);
    assert(result == 0);
    
    /* Test prefix comparison */
    result = strncmp("Testing", "Test", 4);
    assert(result == 0);
    
    result = strncmp("Test", "Testing", 4);
    assert(result == 0);
    
    result = strncmp("Test", "Testing", 5);
    assert(result < 0); /* '\0' < 'i' */
    
    /* Test case sensitivity */
    result = strncmp("HELLO", "hello", 5);
    assert(result < 0);
    
    /* Test partial match */
    result = strncmp("abcdef", "abcxyz", 3);
    assert(result == 0);
    
    result = strncmp("abcdef", "abcxyz", 4);
    assert(result < 0); /* 'd' < 'x' */
    
    /* Test with special characters */
    result = strncmp("Line\n", "Line\t", 4);
    assert(result == 0);
    
    result = strncmp("Line\n", "Line\t", 5);
    assert(result != 0);
    
    /* Test comparing beyond string length */
    result = strncmp("Short", "Short", 100);
    assert(result == 0);
    
    /* Test high ASCII values */
    char str1[] = {(char)0xFF, (char)0xFE, '\0'};
    char str2[] = {(char)0xFF, (char)0xFF, '\0'};
    
    result = strncmp(str1, str2, 2);
    assert(result < 0);
    
    /* Test comparing different lengths */
    result = strncmp("ABC", "ABCDEF", 3);
    assert(result == 0);
    
    result = strncmp("ABCDEF", "ABC", 3);
    assert(result == 0);
    
    /* Test all positions */
    const char *s1 = "0123456789";
    const char *s2 = "0123456789";
    
    for (i = 0; i <= 10; i++) {
        result = strncmp(s1, s2, i);
        assert(result == 0);
    }
    
    /* Test difference at various positions */
    s1 = "abcdefghij";
    s2 = "abcdefghix";
    
    for (i = 0; i <= 8; i++) {
        result = strncmp(s1, s2, i);
        assert(result == 0);
    }
    
    result = strncmp(s1, s2, 9);
    assert(result < 0); /* 'j' < 'x' at position 8 */
    
    TEST_PASSED("strncmp()");
}

/* Test strcoll() function */
void test_strcoll(void) {
    int result;
    char *old_locale;
    
    TEST_START("strcoll()");
    
    /* Save current locale */
    old_locale = setlocale(LC_COLLATE, NULL);
    
    /* Test in "C" locale (should behave like strcmp) */
    setlocale(LC_COLLATE, "C");
    
    /* Test equal strings */
    result = strcoll("Hello", "Hello");
    assert(result == 0);
    
    /* Test different strings */
    result = strcoll("ABC", "XYZ");
    assert(result < 0);
    
    result = strcoll("xyz", "abc");
    assert(result > 0);
    
    /* Test empty strings */
    result = strcoll("", "");
    assert(result == 0);
    
    result = strcoll("Test", "");
    assert(result > 0);
    
    /* Test case differences in C locale */
    result = strcoll("abc", "ABC");
    assert(result > 0); /* lowercase > uppercase in ASCII */
    
    /* Test numeric strings */
    result = strcoll("2", "10");
    assert(result > 0); /* Character comparison, not numeric */
    
    /* Test special characters */
    result = strcoll("A-B", "A B");
    assert(result != 0);
    
    /* Test with punctuation */
    result = strcoll("Hello!", "Hello?");
    assert(result != 0);
    
    /* Test longer strings */
    result = strcoll("The quick brown fox", "The quick brown fox");
    assert(result == 0);
    
    result = strcoll("The quick brown fox", "The quick brown dog");
    assert(result > 0); /* 'f' > 'd' */
    
    /* Try to set a different locale if available */
    if (setlocale(LC_COLLATE, "en_US.UTF-8") != NULL ||
        setlocale(LC_COLLATE, "en_US") != NULL) {
        
        /* Test locale-specific ordering */
        /* Note: Results may vary by locale */
        result = strcoll("café", "cafe");
        /* In some locales, accented chars sort differently */
        
        /* Test case-insensitive in some locales */
        /* This behavior is locale-dependent */
    }
    
    /* Restore original locale */
    setlocale(LC_COLLATE, old_locale);
    
    /* Test that strcoll handles null terminators correctly */
    char s1[] = {'A', 'B', '\0', 'C', '\0'};
    char s2[] = {'A', 'B', '\0', 'D', '\0'};
    
    result = strcoll(s1, s2);
    assert(result == 0); /* Both are "AB" */
    
    TEST_PASSED("strcoll()");
}

/* Test edge cases */
void test_compare_edge_cases(void) {
    int result;
    char str1[BUFFER_SIZE];
    char str2[BUFFER_SIZE];
    size_t i;
    
    TEST_START("string comparison edge cases");
    
    /* Test maximum length identical strings */
    memset(str1, 'X', BUFFER_SIZE - 1);
    memset(str2, 'X', BUFFER_SIZE - 1);
    str1[BUFFER_SIZE - 1] = '\0';
    str2[BUFFER_SIZE - 1] = '\0';
    
    result = strcmp(str1, str2);
    assert(result == 0);
    
    /* Test difference at last position */
    str2[BUFFER_SIZE - 2] = 'Y';
    result = strcmp(str1, str2);
    assert(result < 0);
    
    /* Test all byte values */
    for (i = 1; i < 255; i++) {
        str1[0] = i;
        str1[1] = '\0';
        str2[0] = i + 1;
        str2[1] = '\0';
        
        result = strcmp(str1, str2);
        assert(result < 0);
    }
    
    /* Test strncmp with various n values */
    strcpy(str1, "ABCDEFGHIJ");
    strcpy(str2, "ABCDEFGHIX");
    
    for (i = 0; i <= 11; i++) {
        result = strncmp(str1, str2, i);
        if (i <= 8) {
            assert(result == 0);
        } else {
            assert(result < 0);
        }
    }
    
    /* Test comparing strings with embedded nulls */
    /* Note: This is tricky as strcmp stops at first null */
    memcpy(str1, "AB\0CD", 6);
    memcpy(str2, "AB\0CE", 6);
    
    result = strcmp(str1, str2);
    assert(result == 0); /* Both seen as "AB" */
    
    /* But memcmp would show difference */
    result = memcmp(str1, str2, 5);
    assert(result < 0); /* 'D' < 'E' */
    
    /* Test sign of return value consistency */
    result = strcmp("A", "B");
    assert(result < 0);
    
    result = strcmp("B", "A");
    assert(result > 0);
    
    /* The absolute values don't need to be equal */
    
    TEST_PASSED("Edge case");
}

/* Test common patterns */
void test_compare_patterns(void) {
    int result;
    char buffer[BUFFER_SIZE];
    
    TEST_START("string comparison patterns");
    
    /* Pattern 1: Case-insensitive comparison simulation */
    /* Note: This is a common need but not provided by standard */
    const char *s1 = "Hello";
    const char *s2 = "HELLO";
    
    /* Would need to implement manually or use strcasecmp if available */
    
    /* Pattern 2: Version string comparison */
    result = strcmp("1.9.0", "1.10.0");
    assert(result > 0); /* Wrong for versions! '9' > '1' */
    
    /* Pattern 3: Sorting strings */
    const char *words[] = {"zebra", "apple", "mango", "banana"};
    
    /* Check current order */
    assert(strcmp(words[0], words[1]) > 0); /* zebra > apple */
    assert(strcmp(words[1], words[2]) < 0); /* apple < mango */
    
    /* Pattern 4: Checking prefixes */
    const char *path = "/home/user/file.txt";
    result = strncmp(path, "/home", 5);
    assert(result == 0); /* Has prefix */
    
    /* Pattern 5: Password verification */
    const char *stored = "secret123";
    const char *input = "secret123";
    
    result = strcmp(stored, input);
    assert(result == 0); /* Match */
    
    /* Pattern 6: Sorting with locale */
    setlocale(LC_COLLATE, "C");
    
    /* In C locale, these sort in ASCII order */
    assert(strcoll("A", "a") < 0); /* 'A' < 'a' in ASCII */
    assert(strcoll("Z", "a") < 0); /* 'Z' < 'a' in ASCII */
    
    /* Pattern 7: Finding string in sorted array */
    const char *sorted[] = {"apple", "banana", "cherry", "date", "fig"};
    const char *search = "cherry";
    
    /* Binary search would use strcmp */
    result = strcmp(search, sorted[2]);
    assert(result == 0); /* Found */
    
    /* Pattern 8: Comparing file extensions */
    const char *file1 = "document.txt";
    const char *file2 = "document.doc";
    
    /* Compare last 3 chars */
    result = strcmp(file1 + strlen(file1) - 3, "txt");
    assert(result == 0);
    
    /* Pattern 9: Validating format */
    const char *date = "2023-12-25";
    
    /* Check format */
    assert(date[4] == '-' && date[7] == '-');
    assert(strncmp(date, "2023", 4) == 0);
    
    /* Pattern 10: Menu option comparison */
    const char *option = "quit";
    
    if (strcmp(option, "quit") == 0 || strcmp(option, "exit") == 0) {
        /* Handle quit */
    }
    
    TEST_PASSED("Pattern");
}

/* Test Unicode comparison */
void test_unicode_compare(void) {
    int result;
    
    TEST_START("Unicode string comparison");
    
    /* Note: strcmp does byte-wise comparison, not Unicode-aware */
    
    /* Test UTF-8 strings */
    result = strcmp("café", "café");
    assert(result == 0);
    
    /* Different Unicode normalization could cause issues */
    /* é can be one character or e + combining accent */
    
    /* Test emoji comparison */
    result = strcmp("Hello 😀", "Hello 😀");
    assert(result == 0);
    
    result = strcmp("😀", "😁");
    assert(result != 0);
    
    /* Test mixed scripts */
    result = strcmp("Hello世界", "Hello世界");
    assert(result == 0);
    
    /* Test that comparison is byte-wise */
    result = strcmp("A", "À"); /* A vs A with grave */
    assert(result < 0); /* 'A' = 0x41, 'À' = 0xC3 0x80 */
    
    TEST_PASSED("Unicode comparison");
}

/* Test performance patterns */
void test_compare_performance(void) {
    char str1[1000];
    char str2[1000];
    int result;
    size_t i;
    
    TEST_START("comparison performance patterns");
    
    /* Pattern 1: Early difference detection */
    memset(str1, 'A', 999);
    memset(str2, 'A', 999);
    str1[999] = '\0';
    str2[999] = '\0';
    
    str1[0] = 'B'; /* Difference at start */
    result = strcmp(str1, str2);
    assert(result > 0); /* Should return quickly */
    
    /* Pattern 2: Comparing against multiple strings */
    const char *target = "match";
    const char *candidates[] = {"test", "match", "other"};
    int found = -1;
    
    for (i = 0; i < 3; i++) {
        if (strcmp(target, candidates[i]) == 0) {
            found = i;
            break;
        }
    }
    assert(found == 1);
    
    /* Pattern 3: Using strncmp for partial matching */
    const char *commands[] = {"help", "hello", "history"};
    const char *input = "hel";
    size_t matches = 0;
    
    for (i = 0; i < 3; i++) {
        if (strncmp(input, commands[i], strlen(input)) == 0) {
            matches++;
        }
    }
    assert(matches == 2); /* "help" and "hello" match */
    
    TEST_PASSED("Performance pattern");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 string.h String Comparison Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_strcmp();
    test_strncmp();
    test_strcoll();
    test_compare_edge_cases();
    test_compare_patterns();
    test_unicode_compare();
    test_compare_performance();
    
    TEST_SUITE_PASSED("string comparison");
    return 0;
}
