#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include "ps2_driver.h"
#include "datastruct.h"

#define KEYBOARD_BUFFER_SIZE 256

#define FLAG_CAPS_LOCK        (1 << 0)
#define FLAG_LEFT_SHIFT       (1 << 1)
#define FLAG_RIGHT_SHIFT      (1 << 2)
#define FLAG_LEFT_CTRL        (1 << 3)
#define FLAG_RIGHT_CTRL       (1 << 4)
#define FLAG_LEFT_ALT         (1 << 5)
#define FLAG_RIGHT_ALT        (1 << 6)
#define FLAG_NUM_LK           (1 << 7)
// flags |= FLAG_LEFT_SHIFT : set
// flags &= ~FLAG_LEFT_SHIFT : clear
// if (flags & FLAG_LEFT_SHIFT) : check
// flags ^= FLAGS_NUM_LK : invert

typedef struct key_event
{
    unsigned char make_code;
    unsigned char break_code;    
    unsigned char extended_make_code;
    unsigned char extended_break_code;
}key_event_t;


void irq1_handler(void);
unsigned char get_next_scancode();
unsigned char get_last_scancode();
void KeyEvent();
unsigned char get_last_keycode();
unsigned char GetStackVolume();
unsigned char ScancodeToAscii(unsigned char keycode);
unsigned char GetMakeCode();
unsigned char GetBreakCode();
unsigned char GetExtMakeCode();
unsigned char GetExtBreakCode();
unsigned char GetKeyboardTail();
unsigned char CheckFlags();

#endif
