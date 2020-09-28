#include "compiler.h"

static void compile_ast_internal(const struct ast_node *ast, FILE *file);

static void compile_num(int num, FILE *file)
{
    fprintf(file, "\tmovl\t$%d, %%eax\n", num);
}

static void handle_division(const struct binop_node *bin_op, FILE *file)
{
    compile_ast_internal(bin_op->rhs, file);

    fputs("\tsubq\t$4, %rsp\n", file); // Allocate an `int` on the stack
    fputs("\tmovl\t%eax, (%rsp)\n", file); // Store `rhs` on the stack

    compile_ast_internal(bin_op->lhs, file); // Put `lhs` in `%eax`

    fputs("\tmovl\t(%rsp), %ecx\n", file); // Pop `rhs` in `%edi`
    fputs("\taddq\t$4, %rsp\n", file); // Remove int from the stack

    fputs("\tcdq\n", file); // Sign extend `%eax` into `%edx` and `%eax`
    fputs("\tidivl\t%ecx\n", file); // Divide `eax` by `edi`, store in `eax`
}

static void compile_binop(const struct binop_node* bin_op, FILE *file)
{
    const char *op = NULL;

    if (bin_op->op == DIVIDE)
    {
        handle_division(bin_op, file);
        return;
    }

    compile_ast_internal(bin_op->lhs, file);

    fputs("\tsubq\t$4, %rsp\n", file); // Allocate an `int` on the stack
    fputs("\tmovl\t%eax, (%rsp)\n", file); // Store `lhs` on the stack

    compile_ast_internal(bin_op->rhs, file); // Put `lhs` in `%eax`

    fputs("\tmovl\t(%rsp), %edi\n", file); // Pop `lhs` in `%edi`
    fputs("\taddq\t$4, %rsp\n", file); // Remove int from the stack

    switch (bin_op->op)
    {
    case PLUS:
        op = "addl";
        break;
    case MINUS:
        op = "subl";
        break;
    case TIMES:
        op = "imul";
        break;
    case DIVIDE:
    default:
        /* Not handled */
        break;
    }
    fprintf(file, "\t%s\t%%edi, %%eax\n", op);
}

static void compile_unop(const struct unop_node* un_op, FILE *file)
{
    compile_ast_internal(un_op->rhs, file); // Put `rhs` in `%eax`

    switch (un_op->op)
    {
    case NEGATE:
        fputs("\timul\t$-1, %eax\n", file);
    case IDENTITY:
    default:
        /* Nothing to do */
        break;
    }
}

static void compile_ast_internal(const struct ast_node *ast, FILE *file)
{
    switch (ast->kind)
    {
    case BINOP:
        compile_binop(&ast->val.bin_op, file);
        break;
    case UNOP:
        compile_unop(&ast->val.un_op, file);
        break;
    case NUM:
        compile_num(ast->val.num, file);
        break;
    default:
        break;
    }
}

void compile_ast(const struct ast_node *ast, FILE *file)
{
    fputs("\t.text\n", file); // Executable section
    fputs("\t.globl eval\n", file); // Make symbol visible to `ld`
    fputs("\t.type eval, @function\n", file); // Symbol is a function
    // Write `eval` symbol
    fputs("eval:\n", file);
    // Write function prelude, with the stack frame dance...
    fputs("\tpushq\t%rbp\n", file);
    fputs("\tmovq\t%rsp, %rbp\n", file);

    compile_ast_internal(ast, file); // Compile AST so that it ends in `%eax`

    fputs("\tpopq\t%rbp\n", file); // Finish the stack frame dance...
    fputs("\tret\n", file); // Return value in `%eax`

    fputs("\t.size\teval, .-eval\n", file); // Get size of function
}
