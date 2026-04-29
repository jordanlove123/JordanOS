#include "../include/fb.h"
#include "../include/font.h"
#include "../include/pit.h"

static uint32_t *fb_addr;
static uint32_t  fb_pitch;
static uint32_t  fb_width;
static uint32_t  fb_height;
static uint8_t   fb_bpp;
static uint32_t  fb_cols;
static uint32_t  fb_rows;
static uint32_t  glyph_w;
static uint32_t  glyph_h;
static uint32_t  cur_col = 0;
static uint32_t  cur_row = 0;
static uint32_t  theme_fg = FB_WHITE;
static uint32_t  theme_bg = FB_BLACK;

#define SCREEN_MAX_COLS 128
#define SCREEN_MAX_ROWS 64
static uint8_t screen_buf[SCREEN_MAX_ROWS][SCREEN_MAX_COLS];

void fb_init(mb2_info_t *mbi) {
    mb2_fb_tag_t *tag = (mb2_fb_tag_t *)mb2_find_tag(mbi, MB2_TAG_FB);
    if (!tag) return;
    fb_addr   = (uint32_t *)(uintptr_t)tag->addr;
    fb_pitch  = tag->pitch;
    fb_width  = tag->width;
    fb_height = tag->height;
    fb_bpp    = tag->bpp;
    glyph_w = fb_width / 96;
    if (glyph_w < 8) glyph_w = 8;
    glyph_h = glyph_w * 2;
    fb_cols = fb_width  / glyph_w;
    fb_rows = fb_height / glyph_h;
}

void fb_draw_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t *pixel = (uint32_t *)((uint8_t *)fb_addr + y * fb_pitch + x * (fb_bpp / 8));
    *pixel = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint64_t fb_get_addr(void)   { return (uint64_t)(uintptr_t)fb_addr; }
uint64_t fb_get_size(void)   { return (uint64_t)fb_height * fb_pitch; }
uint32_t fb_get_width(void)  { return fb_width; }
uint32_t fb_get_height(void) { return fb_height; }
uint32_t fb_get_scale(void)  { return glyph_w; }

void fb_clear(uint32_t color) {
    for (uint32_t y = 0; y < fb_height; y++) {
        uint32_t *row = (uint32_t *)((uint8_t *)fb_addr + y * fb_pitch);
        for (uint32_t x = 0; x < fb_width; x++)
            row[x] = color;
    }
}

static void fb_fill_cell(uint32_t col, uint32_t row, uint32_t color) {
    uint8_t r = (color >> 16) & 0xFF, g = (color >> 8) & 0xFF, b = color & 0xFF;
    uint32_t px = col * glyph_w, py = row * glyph_h;
    for (uint32_t sy = 0; sy < glyph_h; sy++)
        for (uint32_t sx = 0; sx < glyph_w; sx++)
            fb_draw_pixel(px + sx, py + sy, r, g, b);
}

static uint32_t last_activity = 0;
#define CURSOR_SOLID_TICKS 30

void fb_draw_cursor(void) {
    if (cur_col < fb_cols && cur_row < fb_rows)
        fb_fill_cell(cur_col, cur_row, theme_fg);
}

void fb_erase_cursor(void) {
    if (cur_col < fb_cols && cur_row < fb_rows)
        fb_fill_cell(cur_col, cur_row, theme_bg);
}

void fb_backspace(void) {
    if (cur_col == 0) return;
    fb_erase_cursor();
    cur_col--;
    fb_fill_cell(cur_col, cur_row, theme_bg);
    if (cur_col < SCREEN_MAX_COLS && cur_row < SCREEN_MAX_ROWS)
        screen_buf[cur_row][cur_col] = 0;
    fb_draw_cursor();
}

void fb_cursor_activity(void) {
    last_activity = pit_get_ticks();
}

void fb_update_cursor(void) {
    uint32_t ticks = pit_get_ticks();
    if (ticks - last_activity < CURSOR_SOLID_TICKS || (ticks / 50) % 2 == 0)
        fb_draw_cursor();
    else
        fb_erase_cursor();
}

static void fb_draw_char(uint32_t col, uint32_t row, char c, uint32_t fg, uint32_t bg) {
    uint8_t fg_r = (fg >> 16) & 0xFF, fg_g = (fg >> 8) & 0xFF, fg_b = fg & 0xFF;
    uint8_t bg_r = (bg >> 16) & 0xFF, bg_g = (bg >> 8) & 0xFF, bg_b = bg & 0xFF;
    const uint8_t *glyph = &font_glyphs[(uint8_t)c * 16];
    uint32_t px = col * glyph_w;
    uint32_t py = row * glyph_h;
    for (uint32_t dy = 0; dy < glyph_h; dy++) {
        uint8_t bits = glyph[dy * 16 / glyph_h];
        for (uint32_t dx = 0; dx < glyph_w; dx++) {
            uint32_t src_x = dx * 8 / glyph_w;
            uint8_t set = (bits & (0x80 >> src_x)) ? 1 : 0;
            fb_draw_pixel(px + dx, py + dy, set ? fg_r : bg_r, set ? fg_g : bg_g, set ? fg_b : bg_b);
        }
    }
    if (col < SCREEN_MAX_COLS && row < SCREEN_MAX_ROWS)
        screen_buf[row][col] = (uint8_t)c;
}

static void fb_scroll(void) {
    uint32_t row_bytes = glyph_h * fb_pitch;
    uint64_t *dst   = (uint64_t *)fb_addr;
    uint64_t *src   = (uint64_t *)((uint8_t *)fb_addr + row_bytes);
    uint64_t  count = (uint64_t)(fb_rows - 1) * row_bytes / 8;
    __asm__ volatile ("rep movsq" : "+D"(dst), "+S"(src), "+c"(count) :: "memory");
    count = row_bytes / 8;
    __asm__ volatile ("rep stosq" : "+D"(dst), "+c"(count) : "a"(0ULL) : "memory");

    uint32_t buf_rows = fb_rows < SCREEN_MAX_ROWS ? fb_rows : SCREEN_MAX_ROWS;
    uint32_t buf_cols = fb_cols < SCREEN_MAX_COLS ? fb_cols : SCREEN_MAX_COLS;
    for (uint32_t r = 0; r < buf_rows - 1; r++)
        for (uint32_t c = 0; c < buf_cols; c++)
            screen_buf[r][c] = screen_buf[r + 1][c];
    for (uint32_t c = 0; c < buf_cols; c++)
        screen_buf[buf_rows - 1][c] = 0;

    cur_row = fb_rows - 1;
}

int fb_get_row(void) { return (int)cur_row; }

void fb_set_row(int row) {
    cur_row = (uint32_t)row;
    if (cur_row >= fb_rows) fb_scroll();
}

void fb_set_col(int col) {
    cur_col = (uint32_t)col;
}

void write_fb(int row, int col, const char *text, uint32_t fg, uint32_t bg) {
    if (row < 0 || (uint32_t)row >= fb_rows || col < 0 || (uint32_t)col >= fb_cols)
        return;
    for (int i = 0; text[i] != '\0' && (uint32_t)(col + i) < fb_cols; i++)
        fb_draw_char((uint32_t)(col + i), (uint32_t)row, text[i], fg, bg);
}

void fb_print_color(const char *text, uint32_t fg, uint32_t bg) {
    fb_erase_cursor();
    for (int i = 0; text[i] != '\0'; i++) {
        if (cur_row >= fb_rows) fb_scroll();
        fb_draw_char(cur_col, cur_row, text[i], fg, bg);
        if (++cur_col >= fb_cols) {
            cur_col = 0;
            cur_row++;
        }
    }
    fb_draw_cursor();
}

void fb_println_color(const char *text, uint32_t fg, uint32_t bg) {
    fb_erase_cursor();
    if (cur_row >= fb_rows) fb_scroll();
    write_fb((int)cur_row, (int)cur_col, text, fg, bg);
    cur_col = 0;
    cur_row++;
    if (cur_row >= fb_rows) fb_scroll();
    fb_draw_cursor();
}

void fb_print(const char *text) {
    fb_print_color(text, theme_fg, theme_bg);
}

void fb_println(const char *text) {
    fb_println_color(text, theme_fg, theme_bg);
}

void fb_clear_screen(void) {
    fb_clear(theme_bg);
    uint32_t buf_rows = fb_rows < SCREEN_MAX_ROWS ? fb_rows : SCREEN_MAX_ROWS;
    uint32_t buf_cols = fb_cols < SCREEN_MAX_COLS ? fb_cols : SCREEN_MAX_COLS;
    for (uint32_t r = 0; r < buf_rows; r++)
        for (uint32_t c = 0; c < buf_cols; c++)
            screen_buf[r][c] = 0;
    cur_row = 0;
    cur_col = 0;
}

void redraw_screen(void) {
    fb_clear(theme_bg);
    uint32_t buf_rows = fb_rows < SCREEN_MAX_ROWS ? fb_rows : SCREEN_MAX_ROWS;
    uint32_t buf_cols = fb_cols < SCREEN_MAX_COLS ? fb_cols : SCREEN_MAX_COLS;
    for (uint32_t r = 0; r < buf_rows; r++)
        for (uint32_t c = 0; c < buf_cols; c++)
            if (screen_buf[r][c])
                fb_draw_char(c, r, (char)screen_buf[r][c], theme_fg, theme_bg);
    fb_draw_cursor();
}

void fb_update_theme(uint32_t fg, uint32_t bg) {
    theme_fg = fg;
    theme_bg = bg;
    redraw_screen();
}

char *addr_to_str(uint64_t addr, char *buf) {
    buf[0] = '0'; buf[1] = 'x';
    for (int i = 15; i >= 0; i--) {
        uint8_t nibble = addr & 0xF;
        buf[2 + i] = nibble < 10 ? '0' + nibble : 'a' + nibble - 10;
        addr >>= 4;
    }
    buf[18] = '\0';
    return buf;
}

char *int_to_str(int n, char *buf) {
    char tmp[11];
    int i = 0, j = 0;
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return buf; }
    if (n < 0)  { buf[j++] = '-'; n = -n; }
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
    return buf;
}
