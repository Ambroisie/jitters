#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "common.h"

#include "ast/ast.h"
#include "eval/evaluator.h"
#include "jit/jitter.h"
#include "parse/parse-jitters.h"

TestSuite(jit, .init = redirect_streams);

#define ERR_STRING "Input '%s' = %d, JIT got %d\n"
void do_compare(const char *input)
{
    write_to_stdin(input);
    struct ast_node *ast = NULL;

    cr_assert_eq(yyparse(&ast), 0);
    cr_assert_not_null(ast);

    int eval = evaluate_ast(ast);
    int jit = jit_eval_ast(ast);

    cr_expect_eq(eval, jit, ERR_STRING, input, eval, jit);

    destroy_ast(ast);
}

Test(jit, one)
{
    do_compare("1");
}

Test(jit, the_answer)
{
    do_compare("42");
}

Test(jit, int_max)
{
    do_compare("2147483647");
}

Test(jit, int_max_plus_one)
{
    do_compare("2147483647 + 1");
}

Test(jit, whitespace)
{
    do_compare("   1   ");
}

Test(jit, more_whitespace)
{
    do_compare("   1   + 2     ");
}

Test(jit, one_plus_one)
{
    do_compare("1+1");
}

Test(jit, one_minus_one)
{
    do_compare("1-1");
}

Test(jit, additions)
{
    do_compare("1+1+1+1+1");
}

Test(jit, substractions)
{
    do_compare("1-1-1-1-1");
}

Test(jit, multiplication)
{
    do_compare("2 * 3");
}

Test(jit, multiplications)
{
    do_compare("1 * 2 * 3 * 4");
}

Test(jit, division)
{
    do_compare("12 / 3");
}

Test(jit, divisions)
{
    do_compare("24 / 4 / 3 / 2");
}

Test(jit, simple_priority)
{
    do_compare("1  + 2 * 3");
}

Test(jit, more_priorities)
{
    do_compare("1 + 6 / 3 + 4 * 6 + 14 / 7");
}

Test(jit, simple_parenthesis)
{
    do_compare("(1 + 2) * 3");
}

Test(jit, more_parenthesis)
{
    do_compare("(1 + 2) * (3 - 4)");
}

Test(jit, unary_minus)
{
    do_compare("-1");
}

Test(jit, unary_plus)
{
    do_compare("+1");
}

Test(jit, unary_torture)
{
    do_compare("--+++--+-+-+-1");
}

Test(jit, altogether)
{
    do_compare("  -   3 + - 4 * 8 / 2 + + 3 -- 2 + ((-1) + 1) * 2 ");
}
