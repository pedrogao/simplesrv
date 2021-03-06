.equ FIBER_STACK_UPPER_OFFSET, 16
.equ FIBER_STACK_PTR_OFFSET, 32
.equ FIBER_STATUS_OFFSET, 40
.equ FIBER_PARENT_OFFSET, 48
.equ FIBER_SCHED_OFFSET, 56

.equ SCHED_CURRENT_FIBER_OFFSET, 0

.equ FIBER_STATUS_EXITED, 0
.equ FIBER_STATUS_INITED, 1

.global ef_fiber_internal_swap
.global ef_fiber_internal_init

.text

ef_fiber_internal_swap:
    mov %rdx, %rax
    push %rbx
    push %rbp
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
    pushfq
    mov %rsp, (%rsi)
    mov %rdi, %rsp

_ef_fiber_restore:
    popfq
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rdi
    pop %rsi
    pop %rbp
    pop %rbx
    ret

_ef_fiber_exit:
    pop %rdx
    mov $FIBER_STATUS_EXITED, %rcx
    mov %rcx, FIBER_STATUS_OFFSET(%rdx)
    mov FIBER_PARENT_OFFSET(%rdx), %rcx
    mov FIBER_SCHED_OFFSET(%rdx), %rdx
    mov %rcx, SCHED_CURRENT_FIBER_OFFSET(%rdx)
    mov FIBER_STACK_PTR_OFFSET(%rcx), %rsp
    jmp _ef_fiber_restore

ef_fiber_internal_init:
    mov $FIBER_STATUS_INITED, %rax
    mov %rax, FIBER_STATUS_OFFSET(%rdi)
    mov %rdi, %rcx
    mov FIBER_STACK_UPPER_OFFSET(%rdi), %rdi
    mov %rcx, -8(%rdi)
    lea _ef_fiber_exit(%rip), %rax
    mov %rax, -16(%rdi)
    mov %rsi, -24(%rdi)
    xor %rax, %rax
    mov %rax, -32(%rdi)
    mov %rdi, -40(%rdi)
    mov %rax, -48(%rdi)
    mov %rdx, -56(%rdi)
    mov %rax, -64(%rdi)
    mov %rax, -72(%rdi)
    mov %rax, -80(%rdi)
    mov %rax, -88(%rdi)
    mov %rax, -96(%rdi)
    mov %rax, -104(%rdi)
    mov %rax, -112(%rdi)
    mov %rax, -120(%rdi)
    mov %rax, -128(%rdi)
    mov %rdi, %rax
    sub $128, %rax
    ret

