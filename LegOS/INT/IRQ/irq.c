#include "../../include/irq.h"

void IRQ4_handler() {
    inb(0x3F8); // COM1 data port — reading it clears some IRQs
    PIC_sendEOI(4);
}