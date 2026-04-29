#include "../include/pic.h"

void pic_init() {
    // Initialize mPIC
    outb(0x20, 0x11);
    io_wait();
    outb(0x21, 0x20);
    io_wait();
    outb(0x21, 0x04);
    io_wait();
    outb(0x21, 0x01);
    io_wait();

    // Initialize sPIC
    outb(0xA0, 0x11);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    // Mask IRQs
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

void pic_eoi(int irq) {
    outb(0x20, 0x20);
    io_wait();
    if (irq > 7)
        outb(0xA0, 0x20);
}

void pic_unmask(int irq) {
    int port = irq < 8 ? 0x21 : 0xA1;
    uint8_t currval = inb(port);
    io_wait();
    uint8_t newval = currval & ~(1 << (irq % 8));
    outb(port, newval);
    io_wait();
}
