#include <stdio.h>

// -------------------------------------------------------------
// |  Argument Type	              | Registers                  |
// -------------------------------------------------------------
// | Integer Arguments 1-6        |	RDI, RSI, RDX, RCX, R8, R9 |
// | Floating Point Arguments 1-8 |	XMM0 - XMM7                |
// | Excess Arguments	          | Stack                      |
// | Static chain pointer         |	R10                        |
// -------------------------------------------------------------
// 参考x86函数调用规约：https://en.wikipedia.org/wiki/X86_calling_conventions
// 函数调用参数，以整型为例，依次存储在 rdi, rsi, rdx, rcx, r8, r9 等 6 个寄存器中
// 函数返回结果，64位整数则存储在 rax 中，128位整数则分开存在 rax，rdx 中
// 如果返回浮点数，则存储在 xmm0 和 xmm1 中
//
// 函数在执行时会频繁使用寄存器，如果上一个函数(caller)在数据暂存在了寄存器中，
// 那么当前函数(callee)会破坏这个暂存的值，因此 caller 和 callee 需要遵循一个
// 规范，为之前的函数保存寄存器数据，这些寄存器数据保存在栈上。
// 寄存器保存规约：
// caller-saved：rax, rcx, rdx, r8, r9, r10, r11
// callee-saved：rbx, rbp, rdi, rsi, rsp, r12, r13, r14, r15
//
// 内联汇编：https://www.cs.utexas.edu/~dahlin/Classes/UGOS/reading/inline.html
//
// 下面、测试一下：

extern long co_swap(long);

int main(int argc, char const *argv[])
{
    int count = 1000;
    for (int i = 0; i < count; i++)
    {
        long ret = co_swap(i);
        printf("%ld\n", ret);
    }

    return 0;
}
