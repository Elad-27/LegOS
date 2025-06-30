#ifndef PIT_H
#define PIT_H

#include "in-out.h"
#include "VGA_driver.h"
#include "stringlib.h"
#include "shapes.h"

// The PIT: Programmable Interval Timer 

#define PIT_CHANNEL0    0x40
#define PIT_COMMAND     0x43
#define PIT_FREQUENCY   1193182 // Base frequency of the PIT in hz

void PIT_SetHz(unsigned int freq);
void timer_callback();
void sleep(unsigned int ticks);
void timer(int seconds);
unsigned int GetTicks();
#endif
