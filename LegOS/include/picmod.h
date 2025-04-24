#ifndef PICMOD_H
#define PICMOD_H

#include <stdint.h>
#include <stddef.h>

void PIC_sendEOI(uint8_t irq);
void PIC_remap(int offset1, int offset2);

#endif