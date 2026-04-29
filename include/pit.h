#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#define TICK_HZ 100

void     pit_init(void);
uint32_t pit_get_ticks(void);
void     inc_ticks(void);

#endif
