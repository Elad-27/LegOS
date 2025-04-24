#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include "ps2_driver.h"  

#define KEYBOARD_BUFFER_SIZE 128

void irq1_handler(void);
unsigned char get_next_keycode();

#endif
