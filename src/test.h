#ifndef TEST_H
#define TEST_H

#include <stdio.h>

struct test_case {
    char *s;
    int a;
    int b;
    int ab;
    int ba;
};

struct test_suite {
    struct test_case *cases;
    size_t count;
    size_t size;
};

struct test_suite *freadtest(FILE *fp);
void test_suite_free(struct test_suite *suite);

#endif // TEST_H