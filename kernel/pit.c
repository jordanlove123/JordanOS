#include "../include/pit.h"
#include "../include/io.h"
#include "../include/pic.h"

static uint32_t tick_count = 0;

void pit_init(void) {
    uint16_t divisor = 1193182 / TICK_HZ;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
    pic_unmask(0);
}

uint32_t pit_get_ticks(void) { return tick_count; }

void inc_ticks(void) { tick_count++; }
