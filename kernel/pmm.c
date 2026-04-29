#include <stdint.h>
#include "../include/pmm.h"
#include "../include/memory.h"

static uint32_t bitmap[32768];
static uint32_t pages_num = 0;
static uint32_t free_pages = 0;

static void bitmap_set(uint32_t page) {
    bitmap[page / 32] |= 1 << (page % 32);
}

static void bitmap_clear(uint32_t page) {
    bitmap[page / 32] &= ~(1 << (page % 32));
}

static int bitmap_test(uint32_t page) {
    return bitmap[(page / 32)] & (1 << (page % 32));
}

void pmm_init(mb2_info_t *mbi) {
    mb2_mmap_entry_t *entry = mmap_first(mbi);
    uint32_t byte_size;
    pages_num = 0;
    free_pages = 0;
    while (entry) {
        byte_size = entry->length;
        if (entry->type == 1) {
            for (int i = 0; i < byte_size / 4096; i++) {
                bitmap_clear(pages_num);
                pages_num++;
                free_pages++;
            }
        }
        else {
            for (int i = 0; i < byte_size / 4096; i++) {
                bitmap_set(pages_num);
                pages_num++;
            }
        }
        entry = mmap_next(mbi, entry);
    }
}

void *pmm_alloc() {
    for (uint32_t i = 0; i < pages_num; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            free_pages--;
            return (void *)(uintptr_t)(i * 4096);
        }
    }

    return 0;
}

void pmm_free(void *addr) {
    free_pages++;
    bitmap_clear((uint32_t)(uintptr_t)addr / 4096);
}

uint32_t pmm_get_total_pages() { return pages_num; }

uint32_t pmm_get_free_pages() { return free_pages; }
