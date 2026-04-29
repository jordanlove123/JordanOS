/*
    This is the malloc implementation of JordanOS. It requests pages from
    physical memory and splits them into chunks with a descriptive header at
    the start in order to allocate memory sized <4 KB. The header has a
    pointer to the previous and next chunk, an in_use marker, and a block
    size field (not including the size of the header). When kmalloc is
    called, it looks for an empty block that's big enough and resizes it if
    it's big enough to be split in two, returning a pointer to that bock
    after the header and marking it as in use. When a pointer is freed,
    the block is marked as not in use and is merged with adjacent blocks if
    they're also free
*/

#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef struct block_header {
    uint32_t size;
    uint8_t  in_use;
    struct block_header *next;
    struct block_header *prev;
} block_header_t;

void heap_init();
void *kmalloc(uint32_t size);
void kfree(void *ptr);

#endif
