#include "printer.h"

#include <stdio.h>

static void printer_binop(const struct binop_node *bin_op)
{
    putchar('(');
    printer_ast(bin_op->lhs);
    switch (bin_op->op)
    {
    case PLUS:
        printf(" + ");
        break;
    case MINUS:
        printf(" - ");
        break;
    case TIMES:
        printf(" * ");
        break;
    case DIVIDE:
        printf(" / ");
        break;
    default:
        break;
    }
    printer_ast(bin_op->rhs);
    putchar(')');
}

static void printer_unop(const struct unop_node *un_op)
{
    putchar('(');
    switch (un_op->op)
    {
    case NEGATE:
        printf("- ");
    case IDENTITY:
    default:
        break;
    }
    printer_ast(un_op->rhs);
    putchar(')');
}

static void printer_num(int num)
{
    printf("%d", num);
}

void printer_ast(const struct ast_node *ast)
{
    switch (ast->kind)
    {
    case BINOP:
        printer_binop(&ast->val.bin_op);
        break;
    case UNOP:
        printer_unop(&ast->val.un_op);
        break;
    case NUM:
        printer_num(ast->val.num);
        break;
    default:
        break;
    }
}
