#include <stdint.h>
#include "../include/vga.h"
#include "../include/io.h"

static int current_row = 0;
static int current_col = 0;

static void vga_scroll();

static void vga_update_cursor() {
    uint16_t pos = (uint16_t)(current_row * 80 + current_col);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

int vga_get_row() {
    return current_row;
}

void vga_set_row(int row) {
    current_row = row;
    if (current_row >= 25)
        vga_scroll();
    vga_update_cursor();
}

void vga_set_col(int col) {
    current_col = col;
    vga_update_cursor();
}

static void vga_scroll() {
    char *vga = (char *)0xB8000;
    /* shift every row up by one — each row is 80 cells * 2 bytes = 160 bytes */
    for (int row = 1; row < 25; row++) {
        for (int col = 0; col < 160; col++)
            vga[(row - 1) * 160 + col] = vga[row * 160 + col];
    }
    /* blank the last row */
    for (int col = 0; col < 80; col++) {
        vga[24 * 160 + col * 2]     = ' ';
        vga[24 * 160 + col * 2 + 1] = 0x00;
    }
    current_row = 24;
}

void write_vga(int row, int col, const char *text, uint8_t text_color, uint8_t bg_color) {
    char *vga = (char *)0xB8000;
    uint8_t color = text_color | (bg_color << 4);
    if (row < 0 || row > 24 || col < 0 || col > 79)
        return;
    int printpos = 2 * (80 * row + col);
    int textpos = 0;
    while (text[textpos] != '\0' && printpos < 2 * 80 * 25) {
        vga[printpos++] = text[textpos];
        vga[printpos++] = color;
        textpos++;
    }
}

void vga_print(const char *text, uint8_t text_color, uint8_t bg_color) {
    if (current_row >= 25)
        vga_scroll();
    write_vga(current_row, current_col, text, text_color, bg_color);
    for (int i = 0; text[i] != '\0'; i++) {
        current_col++;
        if (current_col >= 80) {
            current_col = 0;
            current_row++;
            if (current_row >= 25)
                vga_scroll();
        }
    }
    vga_update_cursor();
}

void vga_println(const char *text, uint8_t text_color, uint8_t bg_color) {
    if (current_row >= 25)
        vga_scroll();
    write_vga(current_row++, 0, text, text_color, bg_color);
    current_col = 0;
    vga_update_cursor();
}

char *addr_to_str(uint32_t addr, char *buf) {
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = addr & 0xF;
        buf[2 + i] = nibble < 10 ? '0' + nibble : 'a' + nibble - 10;
        addr >>= 4;
    }
    buf[10] = '\0';
    return buf;
}

char *int_to_str(int n, char *buf) {
    char tmp[11];
    int i = 0;
    int j = 0;
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return buf; }
    if (n < 0)  { buf[j++] = '-'; n = -n; }
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
    return buf;
}
