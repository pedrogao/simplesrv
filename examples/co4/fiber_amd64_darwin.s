.equ FIBER_STACK_UPPER_OFFSET, 16
.equ FIBER_STACK_PTR_OFFSET, 32
.equ FIBER_STATUS_OFFSET, 40
.equ FIBER_PARENT_OFFSET, 48
.equ FIBER_SCHED_OFFSET, 56

.equ SCHED_CURRENT_FIBER_OFFSET, 0

.equ FIBER_STATUS_EXITED, 0
.equ FIBER_STATUS_INITED, 1

.global _ef_fiber_internal_swap
.global _ef_fiber_internal_init

.text

# new_sp     %rdi
# old_sp_ptr %rsi
# retval     %rdx
_ef_fiber_internal_swap:
    mov %rdx, %rax   # 将第 3 个参数存储到 rax 作为返回值
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
    pushfq           # 保存 rflags
    mov %rsp, (%rsi) # [old_sp_ptr] = %rsp
    mov %rdi, %rsp   # %rsp = new_sp

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

# https://stackoverflow.com/questions/1658294/whats-the-purpose-of-the-lea-instruction
# https://en.wikipedia.org/wiki/X86_calling_conventions
# fiber              %rdi
# fiber_proc         %rsi
# param ｜ fiber     %rdx
_ef_fiber_internal_init:                      # arg0: rdi, arg1: rsi, arg2: rdx, arg3: rcx
    mov $FIBER_STATUS_INITED, %rax            # rax = 1
    mov %rax, FIBER_STATUS_OFFSET(%rdi)       # fiber.status = 1
    mov %rdi, %rcx                            # rcx = fiber
    mov FIBER_STACK_UPPER_OFFSET(%rdi), %rdi  # rdi = fiber.stack_upper
    mov %rcx, -8(%rdi)                        # fiber.stack_area = fiber
    lea _ef_fiber_exit(%rip), %rax            # rax = _ef_fiber_exit
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
    mov %rdi, %rax                           # rax = rdi
    sub $128, %rax                           # return %rax - 128
    ret

