.global idt_flush

.section .text
idt_flush:
    mov 4(%esp), %eax
    lidt (%eax)
    ret
