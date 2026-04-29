#include <stdint.h>
#include "../include/multiboot.h"
#include "../include/memory.h"
#include "../include/pmm.h"
#include "../include/paging.h"
#include "../include/fb.h"
#include "../include/beep.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/keyboard.h"
#include "../include/heap.h"
#include "../include/pit.h"

void kernel_main(uint32_t magic, mb2_info_t *mbi) {
    // beep_n(1);

    pmm_init(mbi);
    fb_init(mbi);
    paging_map_fb(fb_get_addr(), fb_get_size());
    fb_clear(FB_BLACK);

    idt_init();
    heap_init();
    pic_init();
    __asm__ volatile ("sti");
    pit_init();

    fb_clear(FB_BLACK);
    fb_println("Welcome to JordanOS");
    fb_println("Type a command or help");
    fb_print("> ");
    keyboard_init();

    for (;;) {}
}
