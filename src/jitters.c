#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "parse/parse-jitters.h"

int main(void)
{
    struct ast_node *res;
    yydebug = getenv("PARSE") != NULL;
    if (yyparse(&res) == 0) {
        destroy_ast(res);
    }

    return 0;
}
