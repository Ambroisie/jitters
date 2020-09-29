#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "common.h"

#include "ast/ast.h"
#include "eval/evaluator.h"
#include "parse/parse-jitters.h"

TestSuite(evaluate, .init = redirect_streams);

void do_eval(const char *input, int expected)
{
    write_to_stdin(input);
    struct ast_node *ast = NULL;

    cr_assert_eq(yyparse(&ast), 0);
    cr_assert_not_null(ast);

    cr_expect_eq(evaluate_ast(ast), expected);

    destroy_ast(ast);
}

Test(evaluate, one)
{
    do_eval("1", 1);
}

Test(evaluate, the_answer)
{
    do_eval("42", 42);
}

Test(evaluate, int_max)
{
    do_eval("2147483647", 2147483647);
}

Test(evaluate, int_max_plus_one)
{
    do_eval("2147483647 + 1", -2147483648);
}

Test(evaluate, whitespace)
{
    do_eval("   1   ", 1);
}

Test(evaluate, more_whitespace)
{
    do_eval("   1   + 2     ", 3);
}

Test(evaluate, one_plus_one)
{
    do_eval("1+1", 2);
}

Test(evaluate, one_minus_one)
{
    do_eval("1-1", 0);
}

Test(evaluate, additions)
{
    do_eval("1+1+1+1+1", 5);
}

Test(evaluate, substractions)
{
    do_eval("1-1-1-1-1", -3);
}

Test(evaluate, multiplication)
{
    do_eval("2 * 3", 6);
}

Test(evaluate, multiplications)
{
    do_eval("1 * 2 * 3 * 4", 24);
}

Test(evaluate, division)
{
    do_eval("12 / 3", 4);
}

Test(evaluate, divisions)
{
    do_eval("24 / 4 / 3 / 2", 1);
}

Test(evaluate, simple_priority)
{
    do_eval("1  + 2 * 3", 7);
}

Test(evaluate, more_priorities)
{
    do_eval("1 + 6 / 3 + 4 * 6 + 14 / 7", 29);
}

Test(evaluate, simple_parenthesis)
{
    do_eval("(1 + 2) * 3", 9);
}

Test(evaluate, more_parenthesis)
{
    do_eval("(1 + 2) * (3 - 4)", -3);
}

Test(evaluate, unary_minus)
{
    do_eval("-1", -1);
}

Test(evaluate, unary_plus)
{
    do_eval("+1", 1);
}

Test(evaluate, unary_torture)
{
    do_eval("--+++--+-+-+-1", -1);
}

Test(evaluate, altogether)
{
    do_eval("  -   3 + - 4 * 8 / 2 + + 3 -- 2 + ((-1) + 1) * 2 ", -14);
}
