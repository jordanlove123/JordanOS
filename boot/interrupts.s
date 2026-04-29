.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31
.global irq0
.global irq1

.code64
.section .text

/* Exceptions that do NOT push an error code — push dummy 0 first so
 * the stack layout is identical for every handler. */
isr0:  pushq $0; pushq $0;  jmp isr_common
isr1:  pushq $0; pushq $1;  jmp isr_common
isr2:  pushq $0; pushq $2;  jmp isr_common
isr3:  pushq $0; pushq $3;  jmp isr_common
isr4:  pushq $0; pushq $4;  jmp isr_common
isr5:  pushq $0; pushq $5;  jmp isr_common
isr6:  pushq $0; pushq $6;  jmp isr_common
isr7:  pushq $0; pushq $7;  jmp isr_common

/* Exception 8 (double fault) — CPU pushes error code. */
isr8:         pushq $8;  jmp isr_common

isr9:  pushq $0; pushq $9;  jmp isr_common

/* Exceptions 10-14 — CPU pushes error code. */
isr10:        pushq $10; jmp isr_common
isr11:        pushq $11; jmp isr_common
isr12:        pushq $12; jmp isr_common
isr13:        pushq $13; jmp isr_common
isr14:        pushq $14; jmp isr_common

isr15: pushq $0; pushq $15; jmp isr_common
isr16: pushq $0; pushq $16; jmp isr_common

/* Exception 17 (alignment check) — CPU pushes error code. */
isr17:        pushq $17; jmp isr_common

isr18: pushq $0; pushq $18; jmp isr_common
isr19: pushq $0; pushq $19; jmp isr_common
isr20: pushq $0; pushq $20; jmp isr_common

/* Exception 21 (control protection) — CPU pushes error code. */
isr21:        pushq $21; jmp isr_common

isr22: pushq $0; pushq $22; jmp isr_common
isr23: pushq $0; pushq $23; jmp isr_common
isr24: pushq $0; pushq $24; jmp isr_common
isr25: pushq $0; pushq $25; jmp isr_common
isr26: pushq $0; pushq $26; jmp isr_common
isr27: pushq $0; pushq $27; jmp isr_common
isr28: pushq $0; pushq $28; jmp isr_common
isr29: pushq $0; pushq $29; jmp isr_common
isr30: pushq $0; pushq $30; jmp isr_common
isr31: pushq $0; pushq $31; jmp isr_common

irq0:  pushq $0; pushq $32; jmp isr_common
irq1:  pushq $0; pushq $33; jmp isr_common

isr_common:
    /* Stack at entry: int_num, error_code, RIP, CS, RFLAGS, RSP, SS */
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    /* 15 registers * 8 bytes = 120 bytes pushed.
     * int_num is now at 120(%rsp), error_code at 128(%rsp). */
    movq 120(%rsp), %rdi
    movq 128(%rsp), %rsi
    call isr_handler
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    addq $16, %rsp   /* remove int_num and error_code */
    iretq
