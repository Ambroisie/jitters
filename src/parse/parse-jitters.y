%code requires
{
#include "ast/ast.h"
}

%{
#include <stdio.h>
#include "ast/ast.h"

int yylex(void);
void yyerror(struct ast_node **unused, char const *s);
%}

%union
{
    int num;
    struct ast_node *node;
}

%define api.token.prefix {TOK_}
%define parse.error verbose
%define parse.trace true
%parse-param {struct ast_node **ast}
%locations

%token EOF 0 "end-of-file"
%token <num> NUM "number"
%token
     PLUS "+"
     MINUS "-"
     TIMES "*"
     DIVIDE "/"
     LPAREN "("
     RPAREN ")"

%left PLUS MINUS
%left TIMES DIVIDE
%precedence NEG

// Type of our built AST
%type <node> exp
// Use destructor function when discarding tokens
%destructor { destroy_ast($$); } <node>

%%

input:
  exp EOF
  { *ast = $1; }
;

exp:
  NUM
  { $$ = make_num($1); }

| exp PLUS exp
  { $$ = make_binop(PLUS, $1, $3); }
| exp MINUS exp
  { $$ = make_binop(MINUS, $1, $3); }
| exp TIMES exp
  { $$ = make_binop(TIMES, $1, $3); }
| exp DIVIDE exp
  { $$ = make_binop(DIVIDE, $1, $3); }

| PLUS exp %prec NEG
  { $$ = make_unop(IDENTITY, $2); }
| MINUS exp %prec NEG
  { $$ = make_unop(NEGATE, $2); }

| LPAREN exp RPAREN
  { $$ = $2; }
;

%%

void yyerror(struct ast_node **unused, char const *s)
{
    unused = unused; // Unused
    fprintf(stderr, "%s\n", s);
}
