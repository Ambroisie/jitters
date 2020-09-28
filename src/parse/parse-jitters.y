%{
#include <stdio.h>

int yylex(void);
void yyerror(int *unused,char const *s);
%}

%define api.value.type {int}
%define api.token.prefix {TOK_}
%define parse.error verbose
%define parse.trace true
%parse-param {int *result}
%locations

%token EOF 0 "end-of-file"
%token NUM "number"
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

%%

input:
  exp EOF
  { *result = $1; }
;

exp:
  NUM
  { $$ = $1; }

| exp PLUS exp
  { $$ = $1 + $3; }
| exp MINUS exp
  { $$ = $1 - $3; }
| exp TIMES exp
  { $$ = $1 * $3; }
| exp DIVIDE exp
  { $$ = $1 / $3; }

| PLUS exp %prec NEG
  { $$ = $2; }
| MINUS exp %prec NEG
  { $$ = -$2; }

| LPAREN exp RPAREN
  { $$ = $2; }
;

%%

void yyerror(int *unused, char const *s)
{
    unused = unused; // Unused
    fprintf(stderr, "%s\n", s);
}
