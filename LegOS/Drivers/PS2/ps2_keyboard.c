#include "../../include/ps2_keyboard.h"  

unsigned char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int buffer_head = 0;
int buffer_tail = 0;

void irq1_handler(void) {
    unsigned char scancode = inb(DATA_PORT);
    
    int next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    if (next != buffer_tail) {
        keyboard_buffer[buffer_head] = scancode;
        buffer_head = next;
    }

    PIC_sendEOI(1);
}

unsigned char get_next_keycode() {
    if (buffer_head == buffer_tail) {
        return 0; // no key
    }

    unsigned char scancode = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;

    return scancode;
}