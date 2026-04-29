/*
    This is paging, which is the bridge between virtual and physical memory.
    It is done through a page table that the hardware references when given
    a virtual address. Hardware keeps a pointer to a page directory, which
    has up to 1024 entries, each containing a page table. A page table
    contains 1024 entries with physical memory addresses.

    Note: virtual memory addresses are formatted like the following:
    10 bits page directory entry | 10 bits page table entry | 12 bits flags
*/

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

extern uint64_t pdpt[];
extern uint64_t fb_pd[];

void paging_map_fb(uint64_t phys_addr, uint64_t size);

#endif
