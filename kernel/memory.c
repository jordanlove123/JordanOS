#include <stdint.h>
#include "../include/memory.h"

uint32_t mem_get_lower(mb2_info_t *mbi) {
    mb2_mem_tag_t *tag = (mb2_mem_tag_t *)mb2_find_tag(mbi, MB2_TAG_MEM);
    return tag ? tag->mem_lower : 0;
}

uint32_t mem_get_upper(mb2_info_t *mbi) {
    mb2_mem_tag_t *tag = (mb2_mem_tag_t *)mb2_find_tag(mbi, MB2_TAG_MEM);
    return tag ? tag->mem_upper : 0;
}

mb2_mmap_entry_t *mmap_first(mb2_info_t *mbi) {
    mb2_mmap_tag_t *tag = (mb2_mmap_tag_t *)mb2_find_tag(mbi, MB2_TAG_MMAP);
    if (!tag) return 0;
    return (mb2_mmap_entry_t *)((uint8_t *)tag + sizeof(mb2_mmap_tag_t));
}

mb2_mmap_entry_t *mmap_next(mb2_info_t *mbi, mb2_mmap_entry_t *current) {
    mb2_mmap_tag_t *tag = (mb2_mmap_tag_t *)mb2_find_tag(mbi, MB2_TAG_MMAP);
    if (!tag) return 0;
    mb2_mmap_entry_t *next = (mb2_mmap_entry_t *)((uint8_t *)current + tag->entry_size);
    if ((uintptr_t)next >= (uintptr_t)tag + tag->size)
        return 0;
    return next;
}

char *uint_to_hex(uint32_t n, char *buf) {
    const char digits[] = "0123456789ABCDEF";
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 9; i >= 2; i--) {
        buf[i] = digits[n & 0xF];
        n >>= 4;
    }
    buf[10] = '\0';
    return buf;
}

char *size_to_str(uint64_t bytes, char *buf) {
    uint32_t kb = (uint32_t)(bytes / 1024);
    char tmp[11];
    char *p = buf;
    char *num = kb >= 1024 ? uint_to_str(kb / 1024, tmp) : uint_to_str(kb, tmp);
    while (*num) *p++ = *num++;
    *p++ = ' ';
    if (kb >= 1024) { *p++ = 'M'; }
    else            { *p++ = 'K'; }
    *p++ = 'B';
    *p = '\0';
    return buf;
}

char *uint_to_str(uint32_t n, char *buf) {
    char tmp[11];
    int i = 0;
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }
    while (n > 0) {
        tmp[i++] = '0' + (n % 10);
        n /= 10;
    }
    int j = 0;
    while (i > 0)
        buf[j++] = tmp[--i];
    buf[j] = '\0';
    return buf;
}
