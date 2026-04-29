/*
    This is the global descriptor table initialization. This is essentially
    the memory access permissions implemented in hardware as part of
    protected CPU mode. These restrictions aren't needed because they will
    be handled by the kernel, so we basically disable them by giving all
    parts of memory kernel level rwx permissions
*/

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t size;
    uint32_t addr;
} __attribute__((packed)) gdt_descriptor_t;

void gdt_init();
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
extern void gdt_flush(gdt_descriptor_t *desc);

#endif
