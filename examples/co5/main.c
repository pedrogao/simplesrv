#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义一个简单的协程结构体，然后通过汇编 swap context 来交换执行
// https://cfsamson.gitbook.io/green-threads-explained-in-200-lines-of-rust/final-200-lines-of-code
// https://www.cs.uaf.edu/2015/fall/cs301/lecture/11_16_thread_user.html
// https://lldb.llvm.org/use/map.html

#define STACK_SIZE 1024

typedef void (*co_start)(); // 协程入口地址

typedef struct _co_context
{
    long rsp; // 栈指针
    long rbp; // 帧地址
    // callee saved
    long rbx;
    long r12;
    long r13;
    long r14;
    long r15;
    // call stack
    long *stack;
} co_context;

co_context *new_ctx(co_start entry)
{
    co_context *ctx = (co_context *)malloc(sizeof(co_context));
    bzero(ctx, sizeof(co_context));
    // 16 字节对齐
    long *stack = (long *)malloc(STACK_SIZE / sizeof(long));
    ctx->stack = (long *)((long)stack & (long)~15);
    long *base = &ctx->stack[STACK_SIZE / sizeof(long) - 1]; // 栈顶
    if (entry != NULL)
    {
        *(--base) = (long)entry; // return address
    }
    ctx->rsp = (long)base;
    return ctx;
}

void free_ctx(co_context *ctx)
{
    // free(ctx->stack);
    free(ctx);
}

extern void co_swap(co_context *, co_context *);

co_context *ctx1, *ctx2;

void start_b()
{
    printf("start b\n");
    co_swap(ctx2, ctx1); // 再换回去
}

int main(int argc, char const *argv[])
{
    ctx1 = new_ctx(NULL);
    ctx2 = new_ctx(start_b);
    printf("start a\n");
    co_swap(ctx1, ctx2);
    printf("switch back to a\n");
    free_ctx(ctx1);
    free_ctx(ctx2);

    return 0;
}
