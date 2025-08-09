/*
 * test_ctype.c - PSE51 ctype.h character classification test suite
 * Tests: isalnum, isalpha, iscntrl, isdigit, isgraph, islower, isprint,
 *        ispunct, isspace, isupper, isxdigit, tolower, toupper
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for character classification
 */

#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <locale.h>
#include <limits.h>
#include "../test_colors.h"

/* Test isalpha() function */
void test_isalpha(void) {
    int c;
    
    printf("Testing isalpha()...\n");
    
    /* Test uppercase letters */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(isalpha(c) != 0);
    }
    
    /* Test lowercase letters */
    for (c = 'a'; c <= 'z'; c++) {
        assert(isalpha(c) != 0);
    }
    
    /* Test digits - should not be alpha */
    for (c = '0'; c <= '9'; c++) {
        assert(isalpha(c) == 0);
    }
    
    /* Test special characters */
    assert(isalpha(' ') == 0);
    assert(isalpha('\t') == 0);
    assert(isalpha('\n') == 0);
    assert(isalpha('!') == 0);
    assert(isalpha('@') == 0);
    assert(isalpha('#') == 0);
    assert(isalpha('$') == 0);
    assert(isalpha('%') == 0);
    assert(isalpha('^') == 0);
    assert(isalpha('&') == 0);
    assert(isalpha('*') == 0);
    assert(isalpha('(') == 0);
    assert(isalpha(')') == 0);
    assert(isalpha('-') == 0);
    assert(isalpha('_') == 0);
    assert(isalpha('=') == 0);
    assert(isalpha('+') == 0);
    
    /* Test control characters */
    for (c = 0; c < 32; c++) {
        assert(isalpha(c) == 0);
    }
    assert(isalpha(127) == 0); /* DEL */
    
    /* Test EOF */
    assert(isalpha(EOF) == 0);
    
    /* Test high ASCII (implementation-defined) */
    for (c = 128; c < 256; c++) {
        /* May or may not be alpha depending on locale */
    }
    
    printf("  isalpha() tests passed!\n");
}

/* Test isdigit() function */
void test_isdigit(void) {
    int c;
    
    printf("Testing isdigit()...\n");
    
    /* Test digits */
    for (c = '0'; c <= '9'; c++) {
        assert(isdigit(c) != 0);
    }
    
    /* Test non-digits */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(isdigit(c) == 0);
    }
    for (c = 'a'; c <= 'z'; c++) {
        assert(isdigit(c) == 0);
    }
    
    /* Test special characters */
    assert(isdigit(' ') == 0);
    assert(isdigit('.') == 0);
    assert(isdigit(',') == 0);
    assert(isdigit('+') == 0);
    assert(isdigit('-') == 0);
    
    /* Test control characters */
    assert(isdigit('\0') == 0);
    assert(isdigit('\n') == 0);
    assert(isdigit('\t') == 0);
    
    /* Test EOF */
    assert(isdigit(EOF) == 0);
    
    printf("  isdigit() tests passed!\n");
}

/* Test isalnum() function */
void test_isalnum(void) {
    int c;
    
    printf("Testing isalnum()...\n");
    
    /* Test letters */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(isalnum(c) != 0);
    }
    for (c = 'a'; c <= 'z'; c++) {
        assert(isalnum(c) != 0);
    }
    
    /* Test digits */
    for (c = '0'; c <= '9'; c++) {
        assert(isalnum(c) != 0);
    }
    
    /* Test non-alphanumeric */
    assert(isalnum(' ') == 0);
    assert(isalnum('!') == 0);
    assert(isalnum('@') == 0);
    assert(isalnum('#') == 0);
    assert(isalnum('$') == 0);
    assert(isalnum('%') == 0);
    assert(isalnum('^') == 0);
    assert(isalnum('&') == 0);
    assert(isalnum('*') == 0);
    assert(isalnum('(') == 0);
    assert(isalnum(')') == 0);
    assert(isalnum('-') == 0);
    assert(isalnum('_') == 0);
    assert(isalnum('=') == 0);
    assert(isalnum('+') == 0);
    assert(isalnum('[') == 0);
    assert(isalnum(']') == 0);
    assert(isalnum('{') == 0);
    assert(isalnum('}') == 0);
    assert(isalnum('\\') == 0);
    assert(isalnum('|') == 0);
    assert(isalnum(';') == 0);
    assert(isalnum(':') == 0);
    assert(isalnum('\'') == 0);
    assert(isalnum('"') == 0);
    assert(isalnum(',') == 0);
    assert(isalnum('.') == 0);
    assert(isalnum('<') == 0);
    assert(isalnum('>') == 0);
    assert(isalnum('/') == 0);
    assert(isalnum('?') == 0);
    
    /* Test consistency with isalpha and isdigit */
    for (c = 0; c < 256; c++) {
        if (isalpha(c) || isdigit(c)) {
            assert(isalnum(c) != 0);
        } else {
            assert(isalnum(c) == 0);
        }
    }
    
    printf("  isalnum() tests passed!\n");
}

/* Test islower() and isupper() functions */
void test_islower_isupper(void) {
    int c;
    
    printf("Testing islower() and isupper()...\n");
    
    /* Test uppercase letters */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(isupper(c) != 0);
        assert(islower(c) == 0);
    }
    
    /* Test lowercase letters */
    for (c = 'a'; c <= 'z'; c++) {
        assert(islower(c) != 0);
        assert(isupper(c) == 0);
    }
    
    /* Test digits - neither upper nor lower */
    for (c = '0'; c <= '9'; c++) {
        assert(islower(c) == 0);
        assert(isupper(c) == 0);
    }
    
    /* Test special characters */
    assert(islower(' ') == 0);
    assert(isupper(' ') == 0);
    assert(islower('!') == 0);
    assert(isupper('!') == 0);
    
    /* Test EOF */
    assert(islower(EOF) == 0);
    assert(isupper(EOF) == 0);
    
    printf("  islower() and isupper() tests passed!\n");
}

/* Test isspace() function */
void test_isspace(void) {
    int c;
    
    printf("Testing isspace()...\n");
    
    /* Test standard whitespace */
    assert(isspace(' ') != 0);   /* space */
    assert(isspace('\t') != 0);  /* horizontal tab */
    assert(isspace('\n') != 0);  /* newline */
    assert(isspace('\v') != 0);  /* vertical tab */
    assert(isspace('\f') != 0);  /* form feed */
    assert(isspace('\r') != 0);  /* carriage return */
    
    /* Test non-whitespace */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(isspace(c) == 0);
    }
    for (c = 'a'; c <= 'z'; c++) {
        assert(isspace(c) == 0);
    }
    for (c = '0'; c <= '9'; c++) {
        assert(isspace(c) == 0);
    }
    
    /* Test other characters */
    assert(isspace('\0') == 0);
    assert(isspace('!') == 0);
    assert(isspace('.') == 0);
    
    /* Test all ASCII characters */
    int space_chars[] = {' ', '\t', '\n', '\v', '\f', '\r'};
    int is_space_char;
    size_t i, j;
    
    for (c = 0; c < 128; c++) {
        is_space_char = 0;
        for (j = 0; j < sizeof(space_chars)/sizeof(space_chars[0]); j++) {
            if (c == space_chars[j]) {
                is_space_char = 1;
                break;
            }
        }
        
        if (is_space_char) {
            assert(isspace(c) != 0);
        } else {
            assert(isspace(c) == 0);
        }
    }
    
    printf("  isspace() tests passed!\n");
}

/* Test isprint() function */
void test_isprint(void) {
    int c;
    
    printf("Testing isprint()...\n");
    
    /* Test printable ASCII characters (32-126) */
    for (c = 32; c <= 126; c++) {
        assert(isprint(c) != 0);
    }
    
    /* Test control characters (0-31) */
    for (c = 0; c < 32; c++) {
        assert(isprint(c) == 0);
    }
    
    /* Test DEL */
    assert(isprint(127) == 0);
    
    /* Test specific printables */
    assert(isprint(' ') != 0);
    assert(isprint('A') != 0);
    assert(isprint('a') != 0);
    assert(isprint('0') != 0);
    assert(isprint('!') != 0);
    assert(isprint('~') != 0);
    
    /* Test non-printables */
    assert(isprint('\0') == 0);
    assert(isprint('\n') == 0);
    assert(isprint('\t') == 0);
    assert(isprint('\r') == 0);
    
    /* Test EOF */
    assert(isprint(EOF) == 0);
    
    printf("  isprint() tests passed!\n");
}

/* Test isgraph() function */
void test_isgraph(void) {
    int c;
    
    printf("Testing isgraph()...\n");
    
    /* Test graphic characters (33-126) - printable except space */
    for (c = 33; c <= 126; c++) {
        assert(isgraph(c) != 0);
    }
    
    /* Test space - printable but not graphic */
    assert(isgraph(' ') == 0);
    
    /* Test control characters */
    for (c = 0; c < 32; c++) {
        assert(isgraph(c) == 0);
    }
    assert(isgraph(127) == 0);
    
    /* Test consistency with isprint */
    for (c = 0; c < 128; c++) {
        if (isprint(c) && c != ' ') {
            assert(isgraph(c) != 0);
        } else {
            assert(isgraph(c) == 0);
        }
    }
    
    printf("  isgraph() tests passed!\n");
}

/* Test iscntrl() function */
void test_iscntrl(void) {
    int c;
    
    printf("Testing iscntrl()...\n");
    
    /* Test control characters (0-31 and 127) */
    for (c = 0; c < 32; c++) {
        assert(iscntrl(c) != 0);
    }
    assert(iscntrl(127) != 0); /* DEL */
    
    /* Test non-control characters */
    for (c = 32; c < 127; c++) {
        assert(iscntrl(c) == 0);
    }
    
    /* Test specific controls */
    assert(iscntrl('\0') != 0);
    assert(iscntrl('\n') != 0);
    assert(iscntrl('\t') != 0);
    assert(iscntrl('\r') != 0);
    assert(iscntrl('\a') != 0); /* bell */
    assert(iscntrl('\b') != 0); /* backspace */
    
    /* Test non-controls */
    assert(iscntrl(' ') == 0);
    assert(iscntrl('A') == 0);
    assert(iscntrl('0') == 0);
    
    /* Test consistency - control chars are not printable */
    for (c = 0; c < 128; c++) {
        if (iscntrl(c)) {
            assert(isprint(c) == 0);
        }
    }
    
    printf("  iscntrl() tests passed!\n");
}

/* Test ispunct() function */
void test_ispunct(void) {
    int c;
    
    printf("Testing ispunct()...\n");
    
    /* Test known punctuation */
    char punct_chars[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    size_t i;
    
    for (i = 0; punct_chars[i]; i++) {
        assert(ispunct(punct_chars[i]) != 0);
    }
    
    /* Test non-punctuation */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(ispunct(c) == 0);
    }
    for (c = 'a'; c <= 'z'; c++) {
        assert(ispunct(c) == 0);
    }
    for (c = '0'; c <= '9'; c++) {
        assert(ispunct(c) == 0);
    }
    assert(ispunct(' ') == 0);
    assert(ispunct('\t') == 0);
    assert(ispunct('\n') == 0);
    
    /* Test consistency - punct chars are graphic but not alnum */
    for (c = 0; c < 128; c++) {
        if (ispunct(c)) {
            assert(isgraph(c) != 0);
            assert(isalnum(c) == 0);
        }
    }
    
    printf("  ispunct() tests passed!\n");
}

/* Test isxdigit() function */
void test_isxdigit(void) {
    int c;
    
    printf("Testing isxdigit()...\n");
    
    /* Test decimal digits */
    for (c = '0'; c <= '9'; c++) {
        assert(isxdigit(c) != 0);
    }
    
    /* Test uppercase hex digits */
    for (c = 'A'; c <= 'F'; c++) {
        assert(isxdigit(c) != 0);
    }
    
    /* Test lowercase hex digits */
    for (c = 'a'; c <= 'f'; c++) {
        assert(isxdigit(c) != 0);
    }
    
    /* Test non-hex letters */
    for (c = 'G'; c <= 'Z'; c++) {
        assert(isxdigit(c) == 0);
    }
    for (c = 'g'; c <= 'z'; c++) {
        assert(isxdigit(c) == 0);
    }
    
    /* Test other characters */
    assert(isxdigit(' ') == 0);
    assert(isxdigit('x') == 0);
    assert(isxdigit('X') == 0);
    assert(isxdigit('+') == 0);
    assert(isxdigit('-') == 0);
    
    printf("  isxdigit() tests passed!\n");
}

/* Test tolower() function */
void test_tolower(void) {
    int c;
    
    printf("Testing tolower()...\n");
    
    /* Test uppercase to lowercase conversion */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(tolower(c) == c + 32);
    }
    
    /* Test lowercase remains unchanged */
    for (c = 'a'; c <= 'z'; c++) {
        assert(tolower(c) == c);
    }
    
    /* Test digits remain unchanged */
    for (c = '0'; c <= '9'; c++) {
        assert(tolower(c) == c);
    }
    
    /* Test special characters remain unchanged */
    assert(tolower(' ') == ' ');
    assert(tolower('!') == '!');
    assert(tolower('@') == '@');
    assert(tolower('\n') == '\n');
    
    /* Test all ASCII characters */
    for (c = 0; c < 128; c++) {
        if (isupper(c)) {
            assert(tolower(c) == c + 32);
        } else {
            assert(tolower(c) == c);
        }
    }
    
    /* Test EOF */
    assert(tolower(EOF) == EOF);
    
    printf("  tolower() tests passed!\n");
}

/* Test toupper() function */
void test_toupper(void) {
    int c;
    
    printf("Testing toupper()...\n");
    
    /* Test lowercase to uppercase conversion */
    for (c = 'a'; c <= 'z'; c++) {
        assert(toupper(c) == c - 32);
    }
    
    /* Test uppercase remains unchanged */
    for (c = 'A'; c <= 'Z'; c++) {
        assert(toupper(c) == c);
    }
    
    /* Test digits remain unchanged */
    for (c = '0'; c <= '9'; c++) {
        assert(toupper(c) == c);
    }
    
    /* Test special characters remain unchanged */
    assert(toupper(' ') == ' ');
    assert(toupper('!') == '!');
    assert(toupper('@') == '@');
    assert(toupper('\n') == '\n');
    
    /* Test all ASCII characters */
    for (c = 0; c < 128; c++) {
        if (islower(c)) {
            assert(toupper(c) == c - 32);
        } else {
            assert(toupper(c) == c);
        }
    }
    
    /* Test EOF */
    assert(toupper(EOF) == EOF);
    
    /* Test round-trip conversion */
    for (c = 0; c < 128; c++) {
        assert(tolower(toupper(c)) == tolower(c));
        assert(toupper(tolower(c)) == toupper(c));
    }
    
    printf("  toupper() tests passed!\n");
}

/* Test character class relationships */
void test_char_class_relationships(void) {
    int c;
    
    printf("Testing character class relationships...\n");
    
    for (c = 0; c < 128; c++) {
        /* If alpha, then alnum */
        if (isalpha(c)) {
            assert(isalnum(c) != 0);
        }
        
        /* If digit, then alnum and xdigit */
        if (isdigit(c)) {
            assert(isalnum(c) != 0);
            assert(isxdigit(c) != 0);
        }
        
        /* If upper, then alpha */
        if (isupper(c)) {
            assert(isalpha(c) != 0);
        }
        
        /* If lower, then alpha */
        if (islower(c)) {
            assert(isalpha(c) != 0);
        }
        
        /* If control, then not print */
        if (iscntrl(c)) {
            assert(isprint(c) == 0);
        }
        
        /* If graph, then print */
        if (isgraph(c)) {
            assert(isprint(c) != 0);
        }
        
        /* If punct, then graph and print but not alnum */
        if (ispunct(c)) {
            assert(isgraph(c) != 0);
            assert(isprint(c) != 0);
            assert(isalnum(c) == 0);
        }
        
        /* If space, then not graph but might be print */
        if (isspace(c)) {
            assert(isgraph(c) == 0);
            if (c == ' ') {
                assert(isprint(c) != 0);
            }
        }
    }
    
    printf("  Character class relationship tests passed!\n");
}

/* Test with different locales if available */
void test_locale_behavior(void) {
    char *old_locale;
    
    printf("Testing locale behavior...\n");
    
    /* Save current locale */
    old_locale = setlocale(LC_CTYPE, NULL);
    
    /* Test in "C" locale */
    setlocale(LC_CTYPE, "C");
    
    /* In C locale, only ASCII letters are alphabetic */
    assert(isalpha('A') != 0);
    assert(isalpha('z') != 0);
    
    /* High ASCII should not be alphabetic in C locale */
    assert(isalpha(200) == 0);
    
    /* Try other locales if available */
    if (setlocale(LC_CTYPE, "en_US.UTF-8") != NULL ||
        setlocale(LC_CTYPE, "en_US") != NULL) {
        /* Locale-specific behavior may differ */
        /* Some implementations may support extended characters */
    }
    
    /* Restore original locale */
    setlocale(LC_CTYPE, old_locale);
    
    printf("  Locale behavior tests passed!\n");
}

/* Test edge cases */
void test_edge_cases(void) {
    printf("Testing edge cases...\n");
    
    /* Test with unsigned char cast */
    unsigned char uc;
    for (uc = 0; uc < 255; uc++) {
        /* All functions should handle unsigned char values */
        isalpha(uc);
        isdigit(uc);
        tolower(uc);
        toupper(uc);
    }
    
    /* Test boundary values */
    assert(isprint(31) == 0);  /* Before printable range */
    assert(isprint(32) != 0);  /* First printable (space) */
    assert(isprint(126) != 0); /* Last printable (~) */
    assert(isprint(127) == 0); /* After printable range */
    
    /* Test negative values (except EOF) */
    /* Note: Behavior is undefined for negative values other than EOF */
    
    printf("  Edge case tests passed!\n");
}

/* Test common usage patterns */
void test_common_patterns(void) {
    printf("Testing common usage patterns...\n");
    
    /* Pattern 1: String case conversion */
    char str[] = "Hello World 123!";
    char lower[20], upper[20];
    size_t i;
    
    for (i = 0; str[i]; i++) {
        lower[i] = tolower((unsigned char)str[i]);
        upper[i] = toupper((unsigned char)str[i]);
    }
    lower[i] = upper[i] = '\0';
    
    assert(strcmp(lower, "hello world 123!") == 0);
    assert(strcmp(upper, "HELLO WORLD 123!") == 0);
    
    /* Pattern 2: Validate identifier */
    const char *valid_id = "var_123";
    const char *invalid_id = "123var";
    
    /* First char must be letter or underscore */
    assert(isalpha(valid_id[0]) || valid_id[0] == '_');
    assert(!isalpha(invalid_id[0]) && invalid_id[0] != '_');
    
    /* Pattern 3: Skip whitespace */
    const char *text = "   \t\n  Hello";
    const char *p = text;
    while (isspace((unsigned char)*p)) {
        p++;
    }
    assert(strncmp(p, "Hello", 5) == 0);
    
    /* Pattern 4: Extract digits */
    const char *mixed = "abc123def456";
    char digits[20];
    int j = 0;
    
    for (i = 0; mixed[i]; i++) {
        if (isdigit((unsigned char)mixed[i])) {
            digits[j++] = mixed[i];
        }
    }
    digits[j] = '\0';
    assert(strcmp(digits, "123456") == 0);
    
    /* Pattern 5: Count character types */
    const char *sample = "Hello123!@#";
    int alpha_count = 0, digit_count = 0, punct_count = 0;
    
    for (i = 0; sample[i]; i++) {
        if (isalpha((unsigned char)sample[i])) alpha_count++;
        else if (isdigit((unsigned char)sample[i])) digit_count++;
        else if (ispunct((unsigned char)sample[i])) punct_count++;
    }
    
    assert(alpha_count == 5);  /* Hello */
    assert(digit_count == 3);  /* 123 */
    assert(punct_count == 3);  /* !@# */
    
    printf("  Common pattern tests passed!\n");
}

/* Run all tests */
void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 ctype.h Test Suite ===" COLOR_RESET "\n\n");
    
    test_isalpha();
    test_isdigit();
    test_isalnum();
    test_islower_isupper();
    test_isspace();
    test_isprint();
    test_isgraph();
    test_iscntrl();
    test_ispunct();
    test_isxdigit();
    test_tolower();
    test_toupper();
    test_char_class_relationships();
    test_locale_behavior();
    test_edge_cases();
    test_common_patterns();
    
    TEST_SUITE_PASSED("ctype.h");
}
