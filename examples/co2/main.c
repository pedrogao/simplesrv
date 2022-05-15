#include <stdio.h>
#include <stdlib.h>

// 定义一个简单的协程结构体，然后通过汇编 swap context 来交换执行
// https://cfsamson.gitbook.io/green-threads-explained-in-200-lines-of-rust/final-200-lines-of-code
// https://www.cs.uaf.edu/2015/fall/cs301/lecture/11_16_thread_user.html
// https://lldb.llvm.org/use/map.html

#define STACK_SIZE 1024

typedef void (*co_start)(); // 协程入口地址

typedef struct _co_context
{
    long *sp; // 栈指针
    long *stack;
} co_context;

co_context *new_ctx(co_start entry)
{
    co_context *ctx = (co_context *)malloc(sizeof(co_context));
    ctx->stack = malloc(STACK_SIZE / sizeof(long));

    long *base = &ctx->stack[STACK_SIZE / sizeof(long) - 1]; // 栈顶
    ctx->sp = (long *)base;
    if (entry != NULL)
    {
        *(--ctx->sp) = (long)entry; // return address
    }

    return ctx;
}

void free_ctx(co_context *ctx)
{
    free(ctx->stack);
    free(ctx);
}

extern void co_swap(co_context *, co_context *);

co_context *ctx1, *ctx2;

void start_b()
{
    printf("B\n");
    co_swap(ctx2, ctx1); // 再换回去
}

int main(int argc, char const *argv[])
{
    ctx1 = new_ctx(NULL);
    ctx2 = new_ctx(start_b);

    printf("swap before, ctx1.rsp: %ld, ctx2.rsp: %ld\n", *ctx1->sp, *ctx2->sp);
    co_swap(ctx1, ctx2);
    printf("swap before, ctx1.rsp: %ld, ctx2.rsp: %ld\n", *ctx1->sp, *ctx2->sp);

    free_ctx(ctx1);
    free_ctx(ctx2);

    return 0;
}
