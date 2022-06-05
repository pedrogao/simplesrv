    # 全局申明函数
    .global _swap_context

    # 代码段
    .text

# struct Context
# {
#     long rsp; // 栈指针
#     long rbp; // 帧地址
#     // callee saved
#     long rbx;
#     long r12;
#     long r13;
#     long r14;
#     long r15;
# }   

# callee-saved：rbx, rbp, rdi, rsi, rsp, r12, r13, r14, r15
# 将寄存器值存储到栈中
# 参数序号：rdi, rsi，rdx, rcx
# 参数1: old_ctx, 参数2: new_ctx
_swap_context:
    # 存 rsp，旧栈
    movq %rsp, 0(%rdi)
    movq %rbp, 8(%rdi)
    movq %rbx, 16(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 32(%rdi)
    movq %r14, 40(%rdi)
    movq %r15, 48(%rdi)
    # 换栈
    movq 0(%rsi),  %rsp
    movq 8(%rsi),  %rbp
    movq 16(%rsi), %rbx
    movq 24(%rsi), %r12
    movq 32(%rsi), %r13
    movq 40(%rsi), %r14
    movq 48(%rsi), %r15
    # retq 相当于下面两条命令 
    # popq %rip   #恢复指令指针寄存器
    # jmp %rip
    retq
