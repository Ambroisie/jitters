#include "evaluator.h"

static int evaluate_unop(const struct unop_node *un_op)
{
    switch (un_op->op)
    {
    case IDENTITY:
        return evaluate_ast(un_op->rhs);
    case NEGATE:
        return -evaluate_ast(un_op->rhs);
    default:
        return 0;
    }
}

static int evaluate_binop(const struct binop_node *bin_op)
{
    switch (bin_op->op)
    {
    case PLUS:
        return evaluate_ast(bin_op->lhs) + evaluate_ast(bin_op->rhs);
    case MINUS:
        return evaluate_ast(bin_op->lhs) - evaluate_ast(bin_op->rhs);
    case TIMES:
        return evaluate_ast(bin_op->lhs) * evaluate_ast(bin_op->rhs);
    case DIVIDE:
        return evaluate_ast(bin_op->lhs) / evaluate_ast(bin_op->rhs);
    default:
        return 0;
    }
}

int evaluate_ast(const struct ast_node *ast)
{
    switch (ast->kind)
    {
    case NUM:
        return ast->val.num;
    case UNOP:
        return evaluate_unop(&ast->val.un_op);
    case BINOP:
        return evaluate_binop(&ast->val.bin_op);
    default:
        return 0;
    }
}
