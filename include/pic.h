/*
    This is the programmable interrupt controller initialization. It's job
    is to ensure that hardware interrupts are handled correctly. Hardware
    interrupts are given to the PIC which by default directs them to
    interrupts numbered 8-23. This is bad because they will call existing
    CPU interrupt handlers instead of their own handler function, so we need
    to tell the PIC to redirect them to unused IDT entries. These are
    typically numbers 33-48, which is what we do here.
*/

#ifndef PIC_H
#define PIC_H
#include "io.h"

void pic_init();
void pic_eoi(int irq);
void pic_unmask(int irq);

#endif
