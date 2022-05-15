    # 全局申明函数
    .global _co_swap

    # 代码段
    .text

# callee-saved：rbx, rbp, rdi, rsi, rsp, r12, r13, r14, r15
# 将寄存器值存储到栈中
# 参数序号：rdi, rsi，rdx, rcx
_co_swap:
    # 存 rsp，旧栈
    movq %rsp, (%rdi)
    # 换栈
    movq (%rsi), %rsp
    # retq 相当于下面两条命令 
    # popq %rip   #恢复指令指针寄存器
    # jmp %rip
    retq
