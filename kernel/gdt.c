#include <stdint.h>
#include "../include/gdt.h"

static gdt_entry_t gdt[3];

void gdt_init() {
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);
    gdt_descriptor_t desc;
    desc.size = sizeof(gdt) - 1;
    desc.addr = (uint32_t)(uintptr_t)gdt;
    gdt_flush(&desc);
}

/*

access byte:
  bit 7: present — must be 1 for valid segments
  bit 6-5: DPL — privilege level (0 = kernel, 3 = user)
  bit 4: descriptor type — 1 for code/data segments
  bit 3: executable — 1 for code, 0 for data
  bit 2: direction/conforming — set to 0
  bit 1: readable/writable — 1 to allow reads (code) or writes (data)
  bit 0: accessed — CPU sets this, initialize to 0

granularity byte:
  bit 7: granularity — 1 means limit is in 4K pages (so 0xFFFFF * 4K = 4 GiB)
  bit 6: size — 1 for 32-bit protected mode
  bit 5-4: 0
  bits 3-0: upper 4 bits of limit

base: starting address of GDT entry
limit: size of GDT entry

*/

void gdt_set_entry(int index, uint32_t base, uint32_t limit,
              uint8_t access, uint8_t granularity) {
    gdt_entry_t *entry = &gdt[index];
    entry->base_low    = (uint16_t)(base & 0xFFFF);
    entry->base_mid    = (uint8_t)((base >> 16) & 0xFF);
    entry->base_high   = (uint8_t)((base >> 24) & 0xFF);
    entry->limit_low   = (uint16_t)(limit & 0xFFFF);
    entry->access      = access;
    entry->granularity = granularity;
}
