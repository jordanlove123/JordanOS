#include <stdint.h>
#include "../include/io.h"
#include "../include/beep.h"

static void speaker_delay(uint32_t iterations) {
    for (volatile uint32_t i = 0; i < iterations; i++);
}

void beep(uint32_t freq, uint32_t duration) {
    uint16_t divisor = (uint16_t)(1193180 / freq);
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t)(divisor & 0xFF));
    outb(0x42, (uint8_t)(divisor >> 8));
    uint8_t tmp = inb(0x61);
    outb(0x61, tmp | 0x03);
    speaker_delay(duration);
    outb(0x61, tmp & ~0x03);
    speaker_delay(duration / 2);
}

void beep_n(int n) {
    for (int i = 0; i < n; i++)
        beep(1000, 1500000000);
}
