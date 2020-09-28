#include "printer.h"

#include <stdio.h>

static void printer_ast(const struct ast_node *ast, FILE *file);

static void printer_binop(const struct binop_node *bin_op, FILE *file)
{
    fputc('(', file);
    printer_ast(bin_op->lhs, file);
    switch (bin_op->op)
    {
    case PLUS:
        fputs(" + ", file);
        break;
    case MINUS:
        fputs(" - ", file);
        break;
    case TIMES:
        fputs(" * ", file);
        break;
    case DIVIDE:
        fputs(" / ", file);
        break;
    default:
        break;
    }
    printer_ast(bin_op->rhs, file);
    fputc(')', file);
}

static void printer_unop(const struct unop_node *un_op, FILE *file)
{
    fputc('(', file);
    switch (un_op->op)
    {
    case NEGATE:
        fputs("- ", file);
    case IDENTITY:
    default:
        break;
    }
    printer_ast(un_op->rhs, file);
    fputc(')', file);
}

static void printer_num(int num, FILE *file)
{
    fprintf(file, "%d", num);
}

static void printer_ast(const struct ast_node *ast, FILE *file)
{
    switch (ast->kind)
    {
    case BINOP:
        printer_binop(&ast->val.bin_op, file);
        break;
    case UNOP:
        printer_unop(&ast->val.un_op, file);
        break;
    case NUM:
        printer_num(ast->val.num, file);
        break;
    default:
        break;
    }
}

void print_ast(const struct ast_node *ast, FILE *file)
{
    printer_ast(ast, file);
    fputc('\n', file);
}
