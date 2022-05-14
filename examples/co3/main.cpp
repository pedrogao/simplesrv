#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 1024 // 栈大小

typedef void (*co_start)(); // 协程入口地址

/*
     the coroutine layout

    |----------------| <- 栈底 rbp
    |     entry      |
    | return address |
    |----------------| <-
    |      base      |
    |      rbp       |
    |----------------| <- 栈顶 rsp
    |                |
    |                |
    |    reserved    |
    |                |
    |                |
    |                |
    |                |
    |                |
    |----------------| <- 栈底
*/

class coroutine
{
public:
    long *stack_pointer;
    char *stack;

    coroutine(co_start entry)
    {
        if (entry == NULL)
        {
            stack = NULL;
            stack_pointer = NULL;
            return;
        }

        stack = (char *)malloc(STACK_SIZE);
        char *base = stack + STACK_SIZE; // 栈顶
        stack_pointer = (long *)base;
        stack_pointer -= 1;           // 高地址，必须先-1，然后再赋值
        *stack_pointer = (long)entry; // return address
        stack_pointer -= 1;
        *stack_pointer = (long)base; // rbp，将栈顶指针写入栈中
    }

    ~coroutine()
    {
        if (!stack)
            return;
        free(stack);
        stack = NULL;
    }
};

coroutine *co_a, *co_b;

// pushq   %rbp                # push rbp
// movq    %rsp, %rbp          # rbp = rsp
// movq    %rdi, -8(%rbp)      # rbp[-8] = rdi
// movq    %rsi, -16(%rbp)     # rbp[-16] = rsi
// movq    -8(%rbp), %rax      # rax = rbp[-8]
// movq    -16(%rbp), %rcx     # rcx = rbp[-16]
// movq    (%rcx), %rcx        # rcx = [rcx]
// movq    %rax, -24(%rbp)     # rbp[-24] = rax
// movq    %rcx, %rax          # rax = rcx
// movq    -24(%rbp), %rcx     # rcx = rbp[-24]
// # 核心
// movq    %rsp, (%rcx)        # [rcx] = rsp
// movq    %rax, %rsp          # rsp = rax
// popq    %rbp                # pop rbp
// retq                        # ret

void yield_to(coroutine *old_co, coroutine *co)
{
    // 参考：https://www.cs.utexas.edu/~dahlin/Classes/UGOS/reading/inline.html
    // https://www.cristal.univ-lille.fr/~marquet/ens/ctx/doc/l-ia.html
    // 将 rsp 的值保存到 old_co.stack_pointer 中
    // rsp = rax，新的 rsp 指针
    __asm__(
        "movq %%rsp, %0\n\t"
        "movq %%rax, %%rsp\n\t"
        : "=m"(old_co->stack_pointer) // Memory operand constraint(m)
        : "a"(co->stack_pointer)      // a - load into the rax register
        :);
}

void start_b()
{
    printf("B");
    yield_to(co_b, co_a);
    printf("D");
    yield_to(co_b, co_a);
}

int main(int argc, char const *argv[])
{
    printf("A");
    co_b = new coroutine(start_b);
    co_a = new coroutine(NULL);
    yield_to(co_a, co_b);
    printf("C");
    yield_to(co_a, co_b);
    printf("E\n");

    delete co_a;
    delete co_b;

    return 0;
}
