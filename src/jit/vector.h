#ifndef VECTOR_H
#define VECTOR_H

#include <err.h>
#include <stddef.h>
#include <stdlib.h>

struct vector
{
    unsigned char *buf;
    size_t size;
    size_t capacity;
};

static inline void *xmalloc(size_t size)
{
    void *ret = malloc(size);

    if (ret == NULL)
        err(1, NULL);

    return ret;
}

static inline void *xrealloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if (ret == NULL)
    {
        free(ptr);
        err(1, NULL);
    }

    return ret;
}

static inline struct vector *create_vector(size_t init_cap)
{
    struct vector *vec = xmalloc(sizeof(*vec));

    init_cap = (init_cap ? init_cap : 1);

    vec->buf = xmalloc(init_cap);
    vec->size = 0;
    vec->capacity = init_cap;

    return vec;
}

static inline void destroy_vector(struct vector *vec)
{
    free(vec->buf);
    free(vec);
}

static inline void append_vector(struct vector *vec, unsigned char c)
{
    if (vec->size >= vec->capacity)
        vec->buf = xrealloc(vec->buf, vec->capacity *= 2);

    vec->buf[vec->size++] = c;
}

#define ARR_SIZE(ARR) (sizeof((ARR)) / sizeof((ARR)[0]))

static inline void append_array(
    struct vector *buf, const unsigned char *arr, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        append_vector(buf, arr[i]);
}

#endif /* !VECTOR_H */
