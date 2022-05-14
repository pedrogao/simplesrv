# 全局申明函数
.global _co_swap

# 代码段
.text

# callee-saved：rbx, rbp, rdi, rsi, rsp, r12, r13, r14, r15
# 将寄存器值存储到栈中
# 参数序号：rdi, rsi，rdx, rcx
_co_swap:
    # 第一个参数 rdi 存储上一个 ctx
    mov %rbx, 0x00(%rdi)
    mov %rbp, 0x08(%rdi)
    mov %rdi, 0x10(%rdi)
    mov %rsi, 0x18(%rdi)
    mov %rsp, 0x20(%rdi)
    mov %r12, 0x28(%rdi)
    mov %r13, 0x30(%rdi)
    mov %r14, 0x38(%rdi)
    mov %r15, 0x40(%rdi)
    # 第二个参数 rsi 存储下一个 ctx
    mov 0x00(%rsi), %rbx
    mov 0x08(%rsi), %rbp
    mov 0x10(%rsi), %rdi
    mov 0x18(%rsi), %rsi
    mov 0x20(%rsi), %rsp
    mov 0x28(%rsi), %r12
    mov 0x30(%rsi), %r13
    mov 0x38(%rsi), %r14
    mov 0x40(%rsi), %r15
    # 返回
    ret
