#ifndef PICMOD_H
#define PICMOD_H

#include <stdint.h>
#include <stddef.h>

void PIC_sendEOI(uint8_t irq);
void PIC_remap(int offset1, int offset2);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);
static unsigned short __pic_get_irq_reg(int ocw3);
unsigned short pic_get_irr(void);
unsigned short pic_get_isr(void);

#endif