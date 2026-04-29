#include "../include/paging.h"
#include <stdint.h>

void paging_map_fb(uint64_t phys_addr, uint64_t size) {
    if (!phys_addr || !size) return;

    uint32_t pdpt_idx = (phys_addr >> 30) & 0x1FF;
    uint32_t pd_start = (phys_addr >> 21) & 0x1FF;
    uint64_t base     = phys_addr & ~(uint64_t)0x1FFFFF;
    uint32_t n_pages  = (uint32_t)((size + 0x1FFFFF) >> 21) + 1;

    pdpt[pdpt_idx] = (uint64_t)(uintptr_t)fb_pd | 0x3;

    for (uint32_t i = 0; i < n_pages && (pd_start + i) < 512; i++)
        fb_pd[pd_start + i] = (base + (uint64_t)i * 0x200000) | 0x83;

    uint64_t cr3;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(cr3));
    __asm__ volatile ("mov %0, %%cr3" : : "r"(cr3));
}
