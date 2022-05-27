#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solve.h"
#include "test.h"

#define ARG_COUNT 2

int main(int argc, char *argv[]) {
    if (argc != ARG_COUNT) {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("fopen failed (%s)\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct test_suite *suite = freadtest(fp);
    if (suite == NULL) {
        printf("freadtest failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < suite->count; i++) {
        struct test_case *test = &suite->cases[i];
        printf("%s %d %d %d %d\n", test->s, test->a, test->b, test->ab, test->ba);
        int result = solve(test);
        printf("%d\n", result);
    }

    test_suite_free(suite);

    return EXIT_SUCCESS;
}