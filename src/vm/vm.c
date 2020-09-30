#include "vm.h"

#include "bytecode.h"

static enum bytecode_kind binop_to_code(enum binop_kind op)
{
    switch (op)
    {
    case PLUS:
        return PLUS_CODE;
    case MINUS:
        return MINUS_CODE;
    case TIMES:
        return TIMES_CODE;
    case DIVIDE:
        return DIVIDE_CODE;
    }
    return PLUS_CODE; // To avoid errors...
}

static void bytecompile(struct stack *stack, const struct ast_node *ast)
{
    switch (ast->kind)
    {
    case NUM:
        push_num(stack, ast->val.num);
        break;
    case UNOP:
        if (ast->val.un_op.op == IDENTITY)
            bytecompile(stack, ast->val.un_op.rhs);
        else
        {
            bytecompile(stack, ast->val.un_op.rhs);
            push_num(stack, 0);
            push_op(stack, MINUS_CODE);
        }
        break;
    case BINOP:
        bytecompile(stack, ast->val.bin_op.rhs);
        bytecompile(stack, ast->val.bin_op.lhs);
        push_op(stack, binop_to_code(ast->val.bin_op.op));
        break;
    }
}

static int get_num(const void *op)
{
    const struct bytecode_num *num = op;
    return num->num;
}


static int interpret(struct stack *stack)
{
    const struct bytecode *op = get_op(stack);
    switch (op->kind)
    {
    case NUM_CODE:
        stack->size -= sizeof(struct bytecode_num);
        return get_num(op);
    default:
        break;
    }

    stack->size -= sizeof(struct bytecode);
    int lhs = interpret(stack);
    int rhs = interpret(stack);
    switch (op->kind)
    {
    case PLUS_CODE:
        return lhs + rhs;
    case MINUS_CODE:
        return lhs - rhs;
    case TIMES_CODE:
        return lhs * rhs;
    case DIVIDE_CODE:
        return lhs / rhs;
    default:
        return 0; // Should not happen
    }
}

int bytecompile_eval_ast(const struct ast_node *ast)
{
    struct stack *stack = init_stack();

    bytecompile(stack, ast);
    int ret = interpret(stack);
    destroy_stack(stack);

    return ret;
}
