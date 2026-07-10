#include "../include/ata.h"
#include "../include/io.h"
#include "../include/fb.h"

static ata_drive_t drive;

void ata_init() {
    uint16_t buf[256];
    if (ata_identify(buf) != 0) {
        fb_println("Drive identification failed");
        return;
    }
    for (int i = 0; i < 20; i++) {
        uint16_t word = buf[i + 27];
        drive.model[i * 2] = (char)(word >> 8);
        drive.model[i * 2 + 1] = (char)(word & 0xFF);
    }
    drive.model[40] = '\0';
    for (int i = 39; i >= 0 && drive.model[i] == ' '; i--) {
        drive.model[i] = '\0';
    }
    drive.present = 1;
    uint64_t lba48 = ((uint64_t)buf[103] << 48) | ((uint64_t)buf[102] << 32)
                     | ((uint64_t)buf[101] << 16) | (uint64_t)buf[100];
    uint32_t lba32 = ((uint32_t)buf[61] << 16) | (uint32_t)buf[60];
    drive.total_sectors = lba48 ? lba48 : lba32;
}

int ata_identify(uint16_t *buf) {
    outb(0x1F6, 0xA0);
    outb(0x1F2, 0x00);
    outb(0x1F3, 0x00);
    outb(0x1F4, 0x00);
    outb(0x1F5, 0x00);
    outb(0x1F7, 0xEC);
    if (!inb(0x3F6)) return -1;
    int timeout = 100000;
    while ((inb(0x3F6) & 0x80) && --timeout);
    if (timeout == 0) return -1;
    timeout = 100000;
    while (!(inb(0x3F6) & 0x08) && --timeout);
    if (timeout == 0) return -1;
    __asm__ volatile (
        "rep insw" : :
        "c"(256), "d"(0x1F0), "D"(buf)
    );
    return 0;
}

const ata_drive_t *ata_get_drive() {
    return &drive;
}

void ata_read(uint32_t lba, uint16_t *buf, uint8_t sectors) {
    if (sectors == 0) return;
    while (inb(0x3F6) & 0x80);
    outb(0x1F6, 0xE0 | (lba >> 24));
    outb(0x1F3, (uint8_t)(lba));
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F2, sectors);
    outb(0x1F7, 0x20);
    while (inb(0x3F6) & 0x80);
    while (!(inb(0x3F6) & 0x08));
    for (int i = 0; i < sectors; i++) {
        while (inb(0x3F6) & 0x80);
        while (!(inb(0x3F6) & 0x08));
        __asm__ volatile (
            "rep insw" : :
            "c"(256), "d"(0x1F0), "D"(buf)
        );
        buf += 256;
    }
}
