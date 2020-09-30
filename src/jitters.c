#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <argp.h>

#include "config.h"

#include "ast/ast.h"
#include "compile/compiler.h"
#include "eval/evaluator.h"
#include "jit/jitter.h"
#include "parse/parse-jitters.h"
#include "print/printer.h"
#include "vm/vm.h"

const char *argp_program_version = PACKAGE_STRING;
const char *argp_program_bug_address = "<" PACKAGE_BUGREPORT ">";

static char doc[] =
    PACKAGE_NAME " -- an experiment in interpreters and compilers";


static struct argp_option options[] = {
    {"evaluate", 'e', 0,      0,  "Evaluate input by walking the tree", 0 },
    {"virtual",  'v', 0,      0,  "Compile input bytecode and interpret", 0 },
    {"compile",  'c', 0,      0,  "Compile input to assembly", 0 },
    {"jit",      'j', 0,      0,  "JIT-compile input and evaluate", 0 },
    {"print",    'p', 0,      0,  "Print parsed expression", 0 },
    {"output",   'o', "FILE", 0,  "Output to FILE instead of standard output", 0 },
    {"debug",    'd', 0,      0,  "Emit debug output from parser", 0 },
    { 0 }
};

struct arguments
{
    bool debug; // Whether to activate Bison using debug trace
    bool compile; // Whether to compile the input
    bool jit; // Whether to JIT the input
    bool virtual; // Whether to run on VM
    bool evaluate; // Whether to evaluate the input
    bool print; // Whether to print the input
    const char *output_file; // Where to output
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 'c':
        arguments->compile = true;
        break;
    case 'd':
        arguments->debug = true;
        break;
    case 'e':
        arguments->evaluate = true;
        break;
    case 'j':
        arguments->jit = true;
        break;
    case 'p':
        arguments->print = true;
        break;
    case 'o':
        arguments->output_file = arg;
        break;
    case 'v':
        arguments->virtual = true;
        break;
    case ARGP_KEY_ARG:
        argp_usage(state);
        break;
    case ARGP_KEY_END:
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, 0, doc, 0, 0, 0, };

int main(int argc, char *argv[])
{
    struct ast_node *ast = NULL;
    int ret = 0;
    struct arguments arguments = {
        .output_file = "-",
    };
    FILE *output = stdout;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    yydebug = getenv("PARSE") != NULL || arguments.debug;

    if (strcmp(arguments.output_file, "-"))
        output = fopen(arguments.output_file, "w");

    if ((ret = yyparse(&ast)) == 0) {
        if (arguments.compile)
            compile_ast(ast, output);
        if (arguments.jit)
            fprintf(output, "%d\n", jit_eval_ast(ast));
        if (arguments.evaluate)
            fprintf(output, "%d\n", evaluate_ast(ast));
        if (arguments.virtual)
            fprintf(output, "%d\n", bytecompile_eval_ast(ast));
        if (arguments.print)
            print_ast(ast, output);
    }

    destroy_ast(ast);
    if (strcmp(arguments.output_file, "-"))
        fclose(output);

    return ret;
}
