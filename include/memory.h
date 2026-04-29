#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "multiboot.h"

uint32_t mem_get_lower(mb2_info_t *mbi);
uint32_t mem_get_upper(mb2_info_t *mbi);
mb2_mmap_entry_t *mmap_first(mb2_info_t *mbi);
mb2_mmap_entry_t *mmap_next(mb2_info_t *mbi, mb2_mmap_entry_t *current);

/* Writes the decimal representation of n into buf.
 * buf must be at least 11 bytes. Returns buf. */
char *uint_to_str(uint32_t n, char *buf);

/* Writes n as "0xNNNNNNNN" into buf. buf must be at least 11 bytes. */
char *uint_to_hex(uint32_t n, char *buf);

/* Writes byte count as "NNN MB" or "NNN KB" into buf.
 * buf must be at least 12 bytes. */
char *size_to_str(uint64_t bytes, char *buf);

#endif
