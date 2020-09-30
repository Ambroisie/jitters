#ifndef BYTECODE_H
#define BYTECODE_H

#include <stddef.h>

struct bytecode
{
    enum bytecode_kind
    {
        NUM_CODE,
        PLUS_CODE,
        MINUS_CODE,
        TIMES_CODE,
        DIVIDE_CODE,
    } kind;
};

struct bytecode_num
{
    struct bytecode header;
    int num;
};

struct stack
{
    void *buf;
    size_t size;
    size_t capacity;
};

struct stack *init_stack(void);
void destroy_stack(struct stack *stack);
void push_op(struct stack *stack, enum bytecode_kind kind);
void push_num(struct stack *stack, int val);
struct bytecode *get_op(const struct stack *stack);

#endif /* !BYTECODE_H */
