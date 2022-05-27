#include "freadstr.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define READ_BUFFER_SIZE 64
#define STRING_DEFAULT_SIZE 64

struct string {
    char *str;
    size_t len;
    size_t size;
};

static struct string *string_new();
static struct string *string_grow(struct string *s);
static void string_free(struct string *s);
static int string_append(struct string *s, const char *str, size_t len);



char *freadstr(FILE *fp) {
    if (fp == NULL) {
        return NULL;
    }

    struct string *s = string_new();
    if (s == NULL) {
        return NULL;
    }

    char *buffer = (char *)malloc(READ_BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        string_free(s);
        return NULL;
    }

    while (1) {
        errno = 0;
        if (fgets(buffer, READ_BUFFER_SIZE, fp) == NULL) {
            if (feof(fp)) {
                break;
            }

            if (errno == EINTR) {
                continue;
            }

            string_free(s);
            free(buffer);
            printf("freadstr: fgets failed (%s)\n", strerror(errno));
            return NULL;
        }

        int len = strlen(buffer);
        int finished = 0;

        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
            finished = 1;
        }

        if (string_append(s, buffer, len) == -1) {
            string_free(s);
            free(buffer);
            printf("freadstr: string_append failed\n");
            return NULL;
        }

        if (finished) {
            break;
        }
    }

    char *str = s->str;

    free(buffer);
    free(s);

    return str;
}



static struct string *string_new() {
    struct string *s = (struct string *)malloc(sizeof(struct string));
    if (s == NULL) {
        return NULL;
    }

    s->str = (char *)malloc(STRING_DEFAULT_SIZE * sizeof(char));
    if (s->str == NULL) {
        free(s);
        return NULL;
    }

    s->len = 0;
    s->size = STRING_DEFAULT_SIZE;

    return s;
}

static struct string *string_grow(struct string *s) {
    size_t new_size = s->size * 2;
    char *new_str = (char *)realloc(s->str, new_size * sizeof(char));
    if (new_str == NULL) {
        return NULL;
    }

    s->str = new_str;
    s->size = new_size;

    return s;
}

static void string_free(struct string *s) {
    free(s->str);
    free(s);
}

static int string_append(struct string *s, const char *str, size_t len) {
    if (s->len + len >= s->size) {
        struct string *new_s = string_grow(s);
        if (new_s == NULL) {
            return -1;
        }

        s = new_s;
    }

    memcpy(s->str + s->len, str, len);
    s->len += len;
    s->str[s->len] = '\0';

    return 0;
}