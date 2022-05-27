#include "solve.h"

static int recurse(struct test_case *tc, int a, int b, int ab, int ba, int index);



int solve(struct test_case *tc) {
    return recurse(tc, 0, 0, 0, 0, 0);
}



static int recurse(struct test_case *tc, int a, int b, int ab, int ba, int index) {
    if (tc->a == a && tc->b == b && tc->ab == ab && tc->ba == ba) {
        return 1;
    }

    char c1 = tc->s[index];
    if (c1 == '\0') {
        return 0;
    }

    if (recurse(tc,
        (c1 == 'A') ? a + 1 : a,
        (c1 == 'B') ? b + 1 : b,
        ab, ba, index + 1) == 1) {
        return 1;
    }

    char c2 = tc->s[index + 1];
    if (c2 == '\0') {
        return 0;
    }
    
    if (c1 == 'A' && c2 == 'B') {
        return recurse(tc, a, b, ab + 1, ba, index + 2);
    }

    if (c1 == 'B' && c2 == 'A') {
        return recurse(tc, a, b, ab, ba + 1, index + 2);
    }

    return 0;
}