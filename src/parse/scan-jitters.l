%{
#include "parse-jitters.h"

#define YY_USER_ACTION \
    do { \
        yylloc.first_line = yylloc.last_line; \
        yylloc.first_column = yylloc.last_column; \
        for (size_t i = 0; yytext[i] != '\0'; ++i) { \
            if (yytext[i] == '\n') { \
                yylloc.last_line++; \
                yylloc.last_column = 0; \
            } \
            else { \
                yylloc.last_column++; \
            } \
        } \
    } while (0);
%}

/* Assume single input file */
%option noyywrap
/* Let Flex track line number */
%option yylineno

/* Regex shorthands */
blank [ \t\n]+
int   [0-9]+

%%

"+" return TOK_PLUS;
"-" return TOK_MINUS;
"*" return TOK_TIMES;
"/" return TOK_DIVIDE;
"(" return TOK_LPAREN;
")" return TOK_RPAREN;

{int} {
    yylval = atoi(yytext);
    return TOK_NUM;
}

{blank} { /* Do noting */ }

%%
