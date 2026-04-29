#include "../include/heap.h"
#include "../include/pmm.h"

block_header_t *head;

void heap_init() {
    head = (block_header_t*)pmm_alloc();
    head->size = 4096 - sizeof(block_header_t);
    head->in_use = 0;
    head->next = 0;
    head->prev = 0;
}

void resize_block(block_header_t *block, uint32_t size) {
    block_header_t *new_block = (block_header_t*)((char*)block+sizeof(block_header_t)+size);
    new_block->size = block->size - (size + sizeof(block_header_t));
    new_block->in_use = 0;
    new_block->next = block->next;
    new_block->prev = block;
    if (block->next)
        block->next->prev = new_block;
    block->next = new_block;
    block->size = size;
}

void *kmalloc(uint32_t size) {
    if (size > 4096 - sizeof(block_header_t)) {
        return 0;
    }
    block_header_t *block = head;
    block_header_t *old_block;
    while (block != 0) {
        if (block->size >= size && block->size-size >=
                                   sizeof(block_header_t) + 4) {
            // block should be split
            resize_block(block, size);
            block->in_use = 1;
            return (void*)((char*)block + sizeof(block_header_t));
        }
        else if (block->size >= size) {
            // block is big enough but resizing is unnecessary
            block->in_use = 1;
            return (void*)((char*)block + sizeof(block_header_t));
        }
        old_block = block;
        block = block->next;
    }
    // block wasn't found
    block_header_t *new_block = (block_header_t*)pmm_alloc();
    if (new_block == 0) {
        // not enough memory available
        return 0;
    }
    old_block->next = new_block;
    new_block->prev = old_block;
    new_block->next = 0;
    new_block->size = 4096 - sizeof(block_header_t);
    if (new_block->size-size >= sizeof(block_header_t) + 4) {
        resize_block(new_block, size);
    }
    new_block->in_use = 1;
    return (void*)((char*)new_block + sizeof(block_header_t));
}

void kfree(void *ptr) {
    block_header_t *block = (block_header_t*)((char*)ptr - sizeof(block_header_t));
    block_header_t *prev_block = block->prev;
    block_header_t *next_block = block->next;

    // Check if previous block is free and adjacent and merge
    if (prev_block && (char*)prev_block + sizeof(block_header_t) + prev_block->size == (char*)block &&
            !(prev_block->in_use)) {
        prev_block->size += block->size + sizeof(block_header_t);
        prev_block->next = next_block;
        if (next_block)
            next_block->prev = prev_block;
        block = prev_block;
    }

    // Check if next block is free and adjacent and merge
    if (next_block && (char*)block + sizeof(block_header_t) + block->size == (char*)next_block &&
            !(next_block->in_use)) {
        block->size += next_block->size + sizeof(block_header_t);
        block->next = next_block->next;
        if (block->next)
            block->next->prev = block;
    }

    block->in_use = 0;
}
