/*
    This is the physical memory map setup, the first step in setting up the
    kernel. When the OS first boots, GRUB gives it a multiboot struct that
    contains information about the memory, among other things (ie. what
    memory is available and what is reserved). We use that struct here to
    initialize a bitmap that will represent our physical memory in 4KB pages.
    Each bit in the bitmap represents a page and its value represents whether
    or not it's available. We expose pmm_alloc() and pmm_free() for other
    kernel functions to request physical memory, one page at a time.
*/

#ifndef PMM_H
#define PMM_H

#include "multiboot.h"

void pmm_init(mb2_info_t *mbi);
void *pmm_alloc(void);
void pmm_free(void *addr);

uint32_t pmm_get_total_pages(void);
uint32_t pmm_get_free_pages(void);

#endif
