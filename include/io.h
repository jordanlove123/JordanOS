/*
    IO devices communicate with the OS through IO ports that can be read from
    and written to using the inb and outb assembly commands. This is just a
    wrapper for those commands so they can be called with C functions instead
    of inline assembly
 */

#ifndef IO_H
#define IO_H
#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void io_wait() {
    outb(0x80, 0x00);
}

#endif
