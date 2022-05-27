#include "test.h"
#include <stdlib.h>
#include "freadstr.h"

#define TEST_SUITE_DEFAULT_SIZE 8

static struct test_suite *test_suite_create();
static struct test_suite *test_suite_grow(struct test_suite *suite);
static int test_suite_add(struct test_suite *suite, char *s, int a, int b, int ab, int ba);



struct test_suite *freadtest(FILE *fp) {
    if (fp == NULL) {
        return NULL;
    }

    struct test_suite *suite = test_suite_create();
    if (suite == NULL) {
        return NULL;
    }

    int test_cases;
    if (fscanf(fp, "%d", &test_cases) != 1) {
        test_suite_free(suite);
        printf("freadtest: fscanf failed\n");
        return NULL;
    }
    if (test_cases <= 0) {
        test_suite_free(suite);
        printf("freadtest: test cases must be greater than 0\n");
        return NULL;
    }

    for (int i = 0; i < test_cases; i++) {
        char *s;
        int a, b, ab, ba;

        if (fscanf(fp, "%d %d %d %d\n", &a, &b, &ab, &ba) != 4) {
            test_suite_free(suite);
            printf("freadtest: fscanf failed\n");
            return NULL;
        }

        s = freadstr(fp);
        if (s == NULL) {
            test_suite_free(suite);
            printf("freadtest: freadstr failed\n");
            return NULL;
        }

        if (test_suite_add(suite, s, a, b, ab, ba) == -1) {
            test_suite_free(suite);
            printf("freadtest: test_suite_add failed\n");
            return NULL;
        }
    }

    return suite;
}

void test_suite_free(struct test_suite *suite) {
    if (suite == NULL) {
        return;
    }

    free(suite->cases);
    free(suite);
}



static struct test_suite *test_suite_create() {
    struct test_suite *suite = (struct test_suite *)malloc(sizeof(struct test_suite));
    if (suite == NULL) {
        return NULL;
    }

    suite->cases = (struct test_case *)malloc(TEST_SUITE_DEFAULT_SIZE * sizeof(struct test_case));
    if (suite->cases == NULL) {
        free(suite);
        return NULL;
    }

    suite->count = 0;
    suite->size = TEST_SUITE_DEFAULT_SIZE;

    return suite;
}

static struct test_suite *test_suite_grow(struct test_suite *suite) {
    size_t new_size = suite->size * 2;
    struct test_case *new_cases = (struct test_case *)realloc(suite->cases, new_size * sizeof(struct test_case));
    if (new_cases == NULL) {
        return NULL;
    }

    suite->cases = new_cases;
    suite->size = new_size;

    return suite;
}

static int test_suite_add(struct test_suite *suite, char *s, int a, int b, int ab, int ba) {
    if (suite->count + 1 >= suite->size) {
        if (test_suite_grow(suite) == NULL) {
            return -1;
        }
    }

    suite->cases[suite->count].s = s;
    suite->cases[suite->count].a = a;
    suite->cases[suite->count].b = b;
    suite->cases[suite->count].ab = ab;
    suite->cases[suite->count].ba = ba;

    suite->count++;

    return 0;
}