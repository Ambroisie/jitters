#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "common.h"

#include "ast/ast.h"
#include "parse/parse-jitters.h"

TestSuite(parser, .init = redirect_streams);

static void do_correct(const char *input)
{
    write_to_stdin(input);
    struct ast_node *ast = NULL;

    cr_assert_eq(yyparse(&ast), 0);
    cr_assert_not_null(ast);

    destroy_ast(ast);
}

static void do_incorrect(const char *input)
{
    write_to_stdin(input);
    struct ast_node *ast = NULL;

    cr_expect_neq(yyparse(&ast), 0);
    cr_expect_null(ast);

    destroy_ast(ast);
}

Test(parser, empty)
{
    do_incorrect("");
}

Test(parser, unknown_token)
{
    do_incorrect("@");
}

Test(parser, trailing_operator)
{
    do_incorrect("1 +");
}

Test(parser, trailing_expression)
{
    do_incorrect("1 1");
}

Test(parser, double_operator)
{
    do_incorrect("1 * * 1");
}

Test(parser, one)
{
    do_correct("1");
}

Test(parser, the_answer)
{
    do_correct("42");
}

Test(parser, int_max)
{
    do_correct("2147483647");
}

Test(parser, whitespace)
{
    do_correct("   1   ");
}

Test(parser, more_whitespace)
{
    do_correct("   1   + 2     ");
}

Test(parser, one_plus_one)
{
    do_correct("1+1");
}

Test(parser, one_minus_one)
{
    do_correct("1-1");
}

Test(parser, additions)
{
    do_correct("1+1+1+1+1");
}

Test(parser, substractions)
{
    do_correct("1-1-1-1-1");
}

Test(parser, multiplication)
{
    do_correct("2 * 3");
}

Test(parser, multiplications)
{
    do_correct("1 * 2 * 3 * 4");
}

Test(parser, division)
{
    do_correct("12 / 3");
}

Test(parser, divisions)
{
    do_correct("24 / 4 / 3 / 2");
}
