#include <stdio.h>
#include <stdlib.h>

// TODO 定义一个简单的协程结构体，然后通过汇编 swap context 来交换执行
// https://cfsamson.gitbook.io/green-threads-explained-in-200-lines-of-rust/final-200-lines-of-code

typedef struct _co_context
{
    long rbx;
    long rbp;
    long rdi;
    long rsi;
    long rsp;
    long r12;
    long r13;
    long r14;
    long r15;
} co_context;

extern void co_swap(co_context *, co_context *);

co_context *ctx1, *ctx2;

void start_b()
{
    printf("B");
    co_swap(ctx2, ctx1);
}

int main(int argc, char const *argv[])
{
    ctx1 = (co_context *)malloc(sizeof(co_context));
    ctx2 = (co_context *)malloc(sizeof(co_context));
    ctx2->rsp = (long)start_b;

    printf("swap before, ctx1.rsp: %ld, ctx2.rsp: %ld\n", ctx1->rsp, ctx2->rsp);
    co_swap(ctx1, ctx2);
    printf("swap before, ctx1.rsp: %ld, ctx2.rsp: %ld\n", ctx1->rsp, ctx2->rsp);

    return 0;
}
