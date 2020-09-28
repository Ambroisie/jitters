#include "ast.h"

#include <stdlib.h>
#include <err.h>

static void *xmalloc(size_t size)
{
    void *ret = malloc(size);

    if (ret == NULL)
        err(1, NULL);

    return ret;
}

struct ast_node *make_num(int val)
{
    struct ast_node *ret = xmalloc(sizeof(*ret));

    ret->kind = NUM;
    ret->val.num = val;

    return ret;
}

struct ast_node *make_unop(enum unop_kind op, struct ast_node *rhs)
{
    struct ast_node *ret = xmalloc(sizeof(*ret));

    ret->kind = UNOP;
    ret->val.un_op.op = op;
    ret->val.un_op.rhs = rhs;

    return ret;
}

struct ast_node *make_binop(enum binop_kind op, struct ast_node *lhs,
                            struct ast_node *rhs)
{
    struct ast_node *ret = xmalloc(sizeof(*ret));

    ret->kind = BINOP;
    ret->val.bin_op.op = op;
    ret->val.bin_op.lhs = lhs;
    ret->val.bin_op.rhs = rhs;

    return ret;
}

void destroy_ast(struct ast_node *ast)
{
    if (!ast)
        return;

    if (ast->kind == BINOP)
    {
        destroy_ast(ast->val.bin_op.lhs);
        destroy_ast(ast->val.bin_op.rhs);
    }
    else if (ast->kind == UNOP)
        destroy_ast(ast->val.un_op.rhs);

    free(ast);
}
