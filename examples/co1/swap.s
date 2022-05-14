# 全局申明函数
.global _co_swap

# 代码段
.text

# callee-saved：rbx, rbp, rdi, rsi, rsp, r12, r13, r14, r15
# 将寄存器值存储到栈中
_co_swap:
    mov %rdi, %rax # 将第 1 个参数存储到 rax 作为返回值
    push %rbx
    push %rbp
    push %rdi
    push %rsi
    push %rsp
    push %r12
    push %r13
    push %r14
    push %r15
    # 镜像操作
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rsp
    pop %rsi
    pop %rdi
    pop %rbp
    pop %rbx
    ret
