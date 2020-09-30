#include "bytecode.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

static void *xmalloc(size_t size)
{
    void *ret = malloc(size);

    if (ret == NULL)
        err(1, NULL);

    return ret;
}

static void *xrealloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    if (ret == NULL)
    {
        free(ptr);
        err(1, NULL);
    }

    return ret;
}

struct stack *init_stack(void)
{
    struct stack *ret = xmalloc(sizeof(*ret));

    size_t init_cap = sizeof(struct bytecode_num);

    ret->buf = xmalloc(init_cap);
    ret->capacity = init_cap;
    ret->size = 0;

    return ret;
}

void destroy_stack(struct stack *stack)
{
    free(stack->buf);
    free(stack);
}

static void resize_stack(struct stack *stack)
{
    stack->buf = xrealloc(stack->buf, stack->capacity * 2);

    void *old_op = get_op(stack);
    stack->capacity *= 2;
    void *new_op = get_op(stack);

    memmove(new_op, old_op, stack->size);
}

static void *get_end_op(struct stack *stack)
{
    char *tmp = stack->buf;
    return tmp + stack->capacity - stack->size - sizeof(struct bytecode);
}

static void *get_end_num(struct stack *stack)
{
    char *tmp = stack->buf;
    return tmp + stack->capacity - stack->size - sizeof(struct bytecode_num);
}

void push_op(struct stack *stack, enum bytecode_kind kind)
{
    if (stack->capacity - stack->size < sizeof(struct bytecode))
        resize_stack(stack);

    struct bytecode *op = get_end_op(stack);

    op->kind = kind;

    stack->size += sizeof(*op);
}

void push_num(struct stack *stack, int val)
{
    if (stack->capacity - stack->size < sizeof(struct bytecode_num))
        resize_stack(stack);

    struct bytecode_num *num = get_end_num(stack);

    num->header.kind = NUM_CODE;
    num->num = val;

    stack->size += sizeof(*num);
}

struct bytecode *get_op(const struct stack *stack)
{
    char *tmp = stack->buf;
    void *op = tmp + stack->capacity - stack->size;
    return op;
}
