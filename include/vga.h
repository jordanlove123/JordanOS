#ifndef VGA_H
#define VGA_H

#include <stdint.h>

static const uint8_t black     = 0x00;
static const uint8_t blue      = 0x01;
static const uint8_t green     = 0x02;
static const uint8_t turquoise = 0x03;
static const uint8_t red       = 0x04;
static const uint8_t magenta   = 0x05;
static const uint8_t orange    = 0x06;
static const uint8_t lgray     = 0x07;
static const uint8_t dgray     = 0x08;
static const uint8_t lblue     = 0x09;
static const uint8_t lgreen    = 0x0A;
static const uint8_t cyan      = 0x0B;
static const uint8_t lred      = 0x0C;
static const uint8_t pink      = 0x0D;
static const uint8_t yellow    = 0x0E;
static const uint8_t white     = 0x0F;

void write_vga(int row, int col, const char *text, uint8_t text_color, uint8_t bg_color);
void vga_print(const char *text, uint8_t text_color, uint8_t bg_color);
void vga_println(const char *text, uint8_t text_color, uint8_t bg_color);
int  vga_get_row();
void vga_set_row(int row);
void vga_set_col(int col);

/* Writes the decimal representation of n into buf.
 * buf must be at least 12 bytes (sign + 10 digits + null). Returns buf. */
char *addr_to_str(uint32_t addr, char *buf);
char *int_to_str(int n, char *buf);

#endif
