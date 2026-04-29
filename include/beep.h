#ifndef BEEP_H
#define BEEP_H

#include <stdint.h>

void beep(uint32_t freq, uint32_t duration);
void beep_n(int n);

#endif
