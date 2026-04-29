#include "../include/keyboard.h"
#include "../include/pic.h"
#include "../include/fb.h"
#include "../include/shell.h"

static const char scancode_map[58] = {                                    
      0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
      '-', '=', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',                 
      'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h',              
      'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',             
      'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '                            
};

static const char uppercase_map[58] = {
      0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
      '_', '+', 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
      'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H',
      'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
      'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

static int shift   = 0;
static int stopped = 0;

static char line[256];
static int  line_len = 0;

void keyboard_init() {
    pic_unmask(1);
}

void keyboard_stop(void)  { stopped = 1; }
void keyboard_start(void) { stopped = 0; }

void keyboard_handler() {
    uint8_t code = inb(0x60);
    pic_eoi(1);
    if (stopped) return;
    if (code == 0xAA || code == 0xB6) {
        shift = 0;
        return;
    }
    if (code & (1 << 7))
        return;
    fb_cursor_activity();
    if (code == 0x2A || code == 0x36) {
        shift = 1;
        return;
    }
    if (code == 0x1C) {
        fb_erase_cursor();
        fb_set_row(fb_get_row() + 1);
        fb_set_col(0);
        line[line_len] = '\0';
        line_len = 0;
        shell_exec(line);
        fb_print("> ");
        fb_draw_cursor();
        return;
    }
    if (code == 0x0E) {
        if (line_len > 0) {
            line_len--;
            fb_backspace();
        }
        return;
    }
    const char *map = shift ? uppercase_map : scancode_map;
    line[line_len] = map[code];
    line_len++;
    char buf[] = {map[code], '\0'};
    fb_print(buf);
}
