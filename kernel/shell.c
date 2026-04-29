#include "../include/shell.h"
#include "../include/string.h"
#include "../include/fb.h"
#include "../include/io.h"
#include "../include/pmm.h"

void shell_exec(const char *line) {
    static char buf[256];
    char *argv[16];
    int argc = 0;
    int j = 0;

    while (line[j] && j < 255) { buf[j] = line[j]; j++; }
    buf[j] = '\0';

    char *p = buf;
    while (*p && argc < 16) {
        while (*p == ' ') p++;
        if (!*p) break;
        argv[argc++] = p;
        while (*p && *p != ' ') p++;
        if (*p) *p++ = '\0';
    }

    shell_exec_args(argc, argv);
}

void shell_exec_args(int argc, char *argv[]) {
    if (!argc) {
        return;
    }

    if (strcmp(argv[0], "help") == 0) {
        if (argc != 1) {
            fb_println("help command does not take any arguments");
            return;
        }
        fb_println("help - display available commands");
        fb_println("clear - clear the screen");
        fb_println("mem - show memory statistics");
        fb_println("theme <THEME> - change colors");
        fb_println("reboot - reboot computer");
    }
    else if (strcmp(argv[0], "clear") == 0) {
        if (argc != 1) {
            fb_println("clear command does not take any arguments");
            return;
        }
        fb_clear_screen();
    }
    else if (strcmp(argv[0], "mem") == 0) {
        if (argc != 1) {
            fb_println("mem command does not take any arguments");
            return;
        }
        char buf[16];
        uint32_t total_pages = pmm_get_total_pages();
        uint32_t free_pages = pmm_get_free_pages();
        int_to_str((total_pages*4096) >> 20, buf);
        fb_print("Total memory available  ");
        fb_print(buf);
        fb_println(" MB");
        int_to_str(free_pages, buf);
        fb_print("Free pages              ");
        fb_println(buf);
        int_to_str(total_pages-free_pages, buf);
        fb_print("Used pages              ");
        fb_println(buf);
    }
    else if (strcmp(argv[0], "theme") == 0) {
        if (argc != 2) {
            fb_println("usage: theme <THEME> OR theme --help");
            return;
        }
        if (strcmp(argv[1], "--help") == 0) {
            fb_println("Theme        Text          Background");
            fb_println("-------------------------------------");
            fb_println("default      white         black");
            fb_println("hacker       green         black");
            fb_println("amber        amber         black");
            fb_println("ocean        light blue    navy blue");
            fb_println("paper        black         off-white");
            fb_println("rose         pink          purple");
            fb_println("solarized    muted blue    dark teal");
        }
        else if (strcmp(argv[1], "default") == 0) {
            fb_update_theme(FB_WHITE, FB_BLACK);
        }
        else if (strcmp(argv[1], "hacker") == 0) {
            fb_update_theme(0x00FF00, FB_BLACK);
        }
        else if (strcmp(argv[1], "amber") == 0) {
            fb_update_theme(0xFFB000, FB_BLACK);
        }
        else if (strcmp(argv[1], "ocean") == 0) {
            fb_update_theme(0xADD8E6, 0x00001A);
        }
        else if (strcmp(argv[1], "paper") == 0) {
            fb_update_theme(0x1A1A1A, 0xF5F0E8);
        }
        else if (strcmp(argv[1], "rose") == 0) {
            fb_update_theme(0xFFB3C6, 0x1A0010);
        }
        else if (strcmp(argv[1], "solarized") == 0) {
            fb_update_theme(0x839496, 0x002B36);
        }
        else {
            fb_print("Unknown theme ");
            fb_println(argv[1]);
        }
    }
    else if (strcmp(argv[0], "reboot") == 0) {
        if (argc != 1) {
            fb_println("reboot command does not take any arguments");
            return;
        }
        outb(0x64, 0xFE);
        outb(0xCF9, 0x06);
    }
    else {
        fb_print("Unknown command \"");
        fb_print(argv[0]);
        fb_println("\"");
    }
}
