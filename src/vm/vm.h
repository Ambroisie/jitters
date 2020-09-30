#ifndef VM_H
#define VM_H

#include "ast/ast.h"

int bytecompile_eval_ast(const struct ast_node *ast);

#endif /* !VM_H */
