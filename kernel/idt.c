#include "../include/idt.h"
#include "../include/fb.h"
#include "../include/keyboard.h"
#include "../include/pic.h"
#include "../include/pit.h"

static idt_entry_t idt[256];

static void (*handlers[32])(void) = {
    isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,
    isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};

void idt_init(void) {
    for (int i = 0; i < 32; i++)
        idt_set_entry(i, (uint64_t)(uintptr_t)handlers[i], 0x08, 0x8E);
    idt_set_entry(32, (uint64_t)(uintptr_t)irq0, 0x08, 0x8E);
    idt_set_entry(33, (uint64_t)(uintptr_t)irq1, 0x08, 0x8E);

    idt_descriptor_t desc;
    desc.size = sizeof(idt) - 1;
    desc.addr = (uint64_t)(uintptr_t)idt;
    __asm__ volatile ("lidt %0" : : "m"(desc));
}

__attribute__((noinline))
void idt_set_entry(int vector, uint64_t handler, uint16_t selector,
                   uint8_t type_attr) {
    idt_entry_t *entry = &idt[vector];
    entry->offset_low  = handler & 0xFFFF;
    entry->selector    = selector;
    entry->ist         = 0;
    entry->type_attr   = type_attr;
    entry->offset_mid  = (handler >> 16) & 0xFFFF;
    entry->offset_high = (handler >> 32) & 0xFFFFFFFF;
    entry->reserved    = 0;
}

void isr_handler(int int_num, int code) {
    char buf[20];
    if (int_num == 33) {
        keyboard_handler();
        return;
    }
    if (int_num == 14) {
        uint64_t fault_addr;
        __asm__ volatile ("mov %%cr2, %0" : "=r"(fault_addr));
        write_fb(fb_get_row(), 0,  "Page fault at: ", FB_WHITE, FB_BLACK);
        write_fb(fb_get_row(), 15, addr_to_str(fault_addr, buf), FB_WHITE, FB_BLACK);
        fb_set_row(fb_get_row() + 1);
        return;
    }
    if (int_num == 32) {
        inc_ticks();
        fb_update_cursor();
        pic_eoi(0);
        return;
    }
    write_fb(fb_get_row(), 0,  "CPU interrupt: ", FB_WHITE, FB_BLACK);
    write_fb(fb_get_row(), 15, int_to_str(int_num, buf),  FB_WHITE, FB_BLACK);
    write_fb(fb_get_row(), 18, "code: ",                  FB_WHITE, FB_BLACK);
    write_fb(fb_get_row(), 24, int_to_str(code, buf),     FB_WHITE, FB_BLACK);
    fb_set_row(fb_get_row() + 1);
}
