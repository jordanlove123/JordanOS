#ifndef FB_H
#define FB_H
#include <stdint.h>
#include "multiboot.h"

static const uint32_t FB_BLACK   = 0x000000;
static const uint32_t FB_BLUE    = 0x0000AA;
static const uint32_t FB_GREEN   = 0x00AA00;
static const uint32_t FB_TEAL    = 0x00AAAA;
static const uint32_t FB_RED     = 0xAA0000;
static const uint32_t FB_MAGENTA = 0xAA00AA;
static const uint32_t FB_ORANGE  = 0xAA5500;
static const uint32_t FB_LGRAY   = 0xAAAAAA;
static const uint32_t FB_DGRAY   = 0x555555;
static const uint32_t FB_LBLUE   = 0x5555FF;
static const uint32_t FB_LGREEN  = 0x55FF55;
static const uint32_t FB_CYAN    = 0x55FFFF;
static const uint32_t FB_LRED    = 0xFF5555;
static const uint32_t FB_PINK    = 0xFF55FF;
static const uint32_t FB_YELLOW  = 0xFFFF55;
static const uint32_t FB_WHITE   = 0xFFFFFF;

void     fb_init(mb2_info_t *mbi);
void     fb_draw_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void     fb_clear(uint32_t color);
int      fb_get_row(void);
void     fb_set_row(int row);
void     fb_set_col(int col);
void     write_fb(int row, int col, const char *text, uint32_t fg, uint32_t bg);
void     fb_backspace(void);
void     fb_draw_cursor(void);
void     fb_erase_cursor(void);
void     fb_cursor_activity(void);
void     fb_update_cursor(void);
void     fb_print_color(const char *text, uint32_t fg, uint32_t bg);
void     fb_println_color(const char *text, uint32_t fg, uint32_t bg);
void     fb_print(const char *text);
void     fb_println(const char *text);
void     fb_update_theme(uint32_t fg, uint32_t bg);
void     fb_clear_screen(void);
void     redraw_screen(void);
uint64_t fb_get_addr(void);
uint64_t fb_get_size(void);
uint32_t fb_get_width(void);
uint32_t fb_get_height(void);
uint32_t fb_get_scale(void);
char    *addr_to_str(uint64_t addr, char *buf);
char    *int_to_str(int n, char *buf);

#endif
