#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>

#include "ast/ast.h"

void compile_ast(const struct ast_node *ast, FILE *file);

#endif /* !COMPILER_H */
