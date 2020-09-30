#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "common.h"

#include "ast/ast.h"
#include "eval/evaluator.h"
#include "parse/parse-jitters.h"
#include "vm/vm.h"

TestSuite(vm, .init = redirect_streams);

#define ERR_STRING "Input '%s' = %d, VM got %d\n"
static void do_compare(const char *input)
{
    write_to_stdin(input);
    struct ast_node *ast = NULL;

    cr_assert_eq(yyparse(&ast), 0);
    cr_assert_not_null(ast);

    int eval = evaluate_ast(ast);
    int vm = bytecompile_eval_ast(ast);

    cr_expect_eq(eval, vm, ERR_STRING, input, eval, vm);

    destroy_ast(ast);
}

Test(vm, one)
{
    do_compare("1");
}

Test(vm, the_answer)
{
    do_compare("42");
}

Test(vm, int_max)
{
    do_compare("2147483647");
}

Test(vm, int_max_plus_one)
{
    do_compare("2147483647 + 1");
}

Test(vm, whitespace)
{
    do_compare("   1   ");
}

Test(vm, more_whitespace)
{
    do_compare("   1   + 2     ");
}

Test(vm, one_plus_one)
{
    do_compare("1+1");
}

Test(vm, one_minus_one)
{
    do_compare("1-1");
}

Test(vm, additions)
{
    do_compare("1+1+1+1+1");
}

Test(vm, substractions)
{
    do_compare("1-1-1-1-1");
}

Test(vm, multiplication)
{
    do_compare("2 * 3");
}

Test(vm, multiplications)
{
    do_compare("1 * 2 * 3 * 4");
}

Test(vm, division)
{
    do_compare("12 / 3");
}

Test(vm, divisions)
{
    do_compare("24 / 4 / 3 / 2");
}

Test(vm, simple_priority)
{
    do_compare("1  + 2 * 3");
}

Test(vm, more_priorities)
{
    do_compare("1 + 6 / 3 + 4 * 6 + 14 / 7");
}

Test(vm, simple_parenthesis)
{
    do_compare("(1 + 2) * 3");
}

Test(vm, more_parenthesis)
{
    do_compare("(1 + 2) * (3 - 4)");
}

Test(vm, unary_minus)
{
    do_compare("-1");
}

Test(vm, unary_plus)
{
    do_compare("+1");
}

Test(vm, unary_torture)
{
    do_compare("--+++--+-+-+-1");
}

Test(vm, altogether)
{
    do_compare("  -   3 + - 4 * 8 / 2 + + 3 -- 2 + ((-1) + 1) * 2 ");
}
