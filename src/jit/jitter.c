#include "jitter.h"

#include <err.h>
#include <string.h>
#include <sys/mman.h>

#include "vector.h"

static void jit_ast_internal(const struct ast_node *ast, struct vector *buf);

static void jit_num(int num, struct vector *buf)
{
    // movl $num, %eax
    // 08   NUM's value
    append_vector(buf, 0xb8);
    for (int i = 0; i < 4; ++i)
    {
        const unsigned char c = num & 0xff;
        num >>= 8;
        append_vector(buf, c);
    }
}

static void handle_division(const struct binop_node *bin_op, struct vector *buf)
{
    const unsigned char handle_rhs[] = {
        0x48, 0x83, 0xec, 0x04, // sub $0x4,%rsp
        0x89, 0x04, 0x24, // mov %eax,(%rsp)
    };
    jit_ast_internal(bin_op->rhs, buf);
    append_array(buf, handle_rhs, ARR_SIZE(handle_rhs));

    const unsigned char handle_lhs[] = {
        0x8b, 0x0c, 0x24, // mov (%rsp),%ecx
        0x48, 0x83, 0xc4, 0x04, // add $0x4,%rsp
    };
    jit_ast_internal(bin_op->lhs, buf);
    append_array(buf, handle_lhs, ARR_SIZE(handle_lhs));

    const unsigned char handle_div[] = {
        0x99, // cltd
        0xf7, 0xf9, // idiv %ecx
    };
    append_array(buf, handle_div, ARR_SIZE(handle_div));
}

static void jit_binop(const struct binop_node* bin_op, struct vector *buf)
{

    if (bin_op->op == DIVIDE)
    {
        handle_division(bin_op, buf);
        return;
    }

    const unsigned char handle_rhs[] = {
        0x48, 0x83, 0xec, 0x04, // sub $0x4,%rsp
        0x89, 0x04, 0x24, // mov %eax,(%rsp)
    };
    jit_ast_internal(bin_op->rhs, buf);
    append_array(buf, handle_rhs, ARR_SIZE(handle_rhs));

    const unsigned char handle_lhs[] = {
        0x8b, 0x3c, 0x24, // mov (%rsp),%edi
        0x48, 0x83, 0xc4, 0x04, // add $0x4,%rsp
    };
    jit_ast_internal(bin_op->lhs, buf);
    append_array(buf, handle_lhs, ARR_SIZE(handle_lhs));

    const unsigned char handle_plus[] = {
        0x01, 0xf8, // add %edi,%eax
    };
    const unsigned char handle_minus[] = {
        0x29, 0xf8, // sub %edi,%eax
    };
    const unsigned char handle_times[] = {
        0x0f, 0xaf, 0xc7, // imul %edi,%eax
    };
    switch (bin_op->op)
    {
    case PLUS:
        append_array(buf, handle_plus, ARR_SIZE(handle_plus));
        break;
    case MINUS:
        append_array(buf, handle_minus, ARR_SIZE(handle_minus));
        break;
    case TIMES:
        append_array(buf, handle_times, ARR_SIZE(handle_times));
        break;
    case DIVIDE:
    default:
        /* Not handled */
        break;
    }
}

static void jit_unop(const struct unop_node* un_op, struct vector *buf)
{
    jit_ast_internal(un_op->rhs, buf);

    const unsigned char handle_negate[] = {
        0x6b, 0xc0, 0xff // imul $-1,%eax,%eax
    };
    switch (un_op->op)
    {
    case NEGATE:
        append_array(buf, handle_negate, ARR_SIZE(handle_negate));
        break;
    case IDENTITY:
    default:
        /* Nothing to do */
        break;
    }
}

static void jit_ast_internal(const struct ast_node *ast, struct vector *buf)
{
    switch (ast->kind)
    {
    case BINOP:
        jit_binop(&ast->val.bin_op, buf);
        break;
    case UNOP:
        jit_unop(&ast->val.un_op, buf);
        break;
    case NUM:
        jit_num(ast->val.num, buf);
        break;
    default:
        break;
    }
}

static void jit_ast(const struct ast_node *ast, struct vector *buf)
{
    const unsigned char prologue[] = {
        0x55, // push %rbp
        0x48, 0x89, 0xe5, // mov %rsp,%rbp
    };
    append_array(buf, prologue, ARR_SIZE(prologue));

    jit_ast_internal(ast, buf);

    const unsigned char epilogue[] = {
        0x5d, // pop %rbp
        0xc3, // retq
    };
    append_array(buf, epilogue, ARR_SIZE(epilogue));
}

#define WRITE_PROT (PROT_READ | PROT_WRITE)
#define WRITE_FLAGS (MAP_ANONYMOUS | MAP_PRIVATE)
static int exec_buf(const void *buf, size_t len)
{
    // Copy our JIT-ed instructions into a new buffer
    void *ptr = mmap(NULL, len, WRITE_PROT, WRITE_FLAGS, 0, 0);
    if (ptr == MAP_FAILED)
        err(1, NULL);
    memcpy(ptr, buf, len);

    // Make the JIT-ed function executable
    if (mprotect(ptr, len, PROT_READ | PROT_EXEC) < 0)
        err(1, NULL);

    // Cast our pointer into a function pointer
    // Thanks to (-Wpedantic) we need to do this pragma song and dance...
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    int (*func)(void) = ptr;
#pragma GCC diagnostic pop
    // Call the JIT-ed function
    int ret = func();

    // Un-map the JIT-ed function
    if (munmap(ptr, len) < 0)
        err(1, NULL);

    return ret;
}

int jit_eval_ast(const struct ast_node *ast)
{
    struct vector *buf = create_vector(1);
    jit_ast(ast, buf);

    int ret = exec_buf(buf->buf, buf->size);

    destroy_vector(buf);
    return ret;
}
