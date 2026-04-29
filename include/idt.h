/*
    This is the interrupt descriptor table initialization. This is how
    the operating system handles interrupts sent by the CPU which could be
    either CPU interrupts (eg. div by 0 error), hardware interrupts (eg.
    keyboard input), or software interrupts (eg. syscall from user program).
    The idt tells the OS what to do for a certain interrupt by giving it
    a table that links interrupts to handler functions, so when an interrupt
    is triggered, the handler function is automatically called. CPU
    interrupts must all be handled otherwise they will reboot the computer
    and others can be implemented as the kernel is developed, ie. to add
    keyboard use.
*/

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t size;
    uint64_t addr;
} __attribute__((packed)) idt_descriptor_t;

void idt_init(void);
__attribute__((noinline))
void idt_set_entry(int vector, uint64_t handler, uint16_t selector,
                   uint8_t type_attr);
void isr_handler(int int_num, int error_code);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();

#endif
