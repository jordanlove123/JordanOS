#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MB2_BOOTLOADER_MAGIC 0x36d76289

#define MB2_TAG_END     0
#define MB2_TAG_MEM     4
#define MB2_TAG_MMAP    6
#define MB2_TAG_FB      8

typedef struct {
    uint32_t total_size;
    uint32_t reserved;
} __attribute__((packed)) mb2_info_t;

typedef struct {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) mb2_tag_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t mem_lower;
    uint32_t mem_upper;
} __attribute__((packed)) mb2_mem_tag_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
} __attribute__((packed)) mb2_mmap_tag_t;

typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed)) mb2_mmap_entry_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;
    uint8_t  fb_type;
    uint16_t reserved;
} __attribute__((packed)) mb2_fb_tag_t;

static inline mb2_tag_t *mb2_next_tag(mb2_tag_t *tag) {
    return (mb2_tag_t *)(((uintptr_t)tag + tag->size + 7) & ~7);
}

static inline mb2_tag_t *mb2_find_tag(mb2_info_t *mbi, uint32_t type) {
    mb2_tag_t *tag = (mb2_tag_t *)((uint8_t *)mbi + sizeof(mb2_info_t));
    while (tag->type != MB2_TAG_END) {
        if (tag->type == type)
            return tag;
        tag = mb2_next_tag(tag);
    }
    return 0;
}

#endif
