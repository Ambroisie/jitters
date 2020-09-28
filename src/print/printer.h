#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>

#include "ast/ast.h"

void print_ast(const struct ast_node *ast, FILE *file);

#endif /* !PRINTER_H */
