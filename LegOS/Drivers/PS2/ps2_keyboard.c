#include "../../include/ps2_keyboard.h"  

unsigned char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int buffer_head = 0;
int buffer_tail = 0;

int keyboard_stack[KEYBOARD_BUFFER_SIZE];
int *sp = keyboard_stack;

key_event_t key_code = {
    0,
    0
};

unsigned char flags = 0;
/** 
 ** bit | purpose
 * 0    | caps lock
 * 1    | left shift
 * 2    | right shift
 * 3    | left ctrl
 * 4    | right ctrl
 * 5    | left alt
 * 6    | right alt
 * 7    | num lk
 ** Haha 8-bit long bool array goes brrrr
*/

/**
 * side note: bool type vars are the same size as an unsigned char, this is because the CPU can't reference somthing in memory 
 * that's smaller than 8 bits much less specific bits like 1's and 0's. 
 * So while Bool is used for true or false, 1 or 0, the same can be achived with using an unsigned char with the added bonus of 
 * having multiple bits to use as different flags. 
*/

/**
 ** Important info:
 *  The way the OS handles input from the keyboard is as such:
 * 1) When an IRQ occurs it reads from the PS/2 Controller's Data Port (0x60) and saves it in a queue (the keyboard buffer)
 * 2) When you want to get the next scancode you take out that element from the queue
 * 3) At the same time that we insert the scancode into the queue we also push it into a stack 
 **   This is usfull for keeping track of the keys with the stack but not affecting the queue 
*/

unsigned char ScancodeToAsciiTable_lowercase[256] = {
    [0x0E] = '`',
    [0x15] = 'q',
    [0x16] = '1',
    [0x1A] = 'z',
    [0x1B] = 's',
    [0x1C] = 'a',
    [0x1D] = 'w',
    [0x1E] = '2',
    [0x21] = 'c',
    [0x22] = 'x',
    [0x23] = 'd',
    [0x24] = 'e',
    [0x25] = '4',
    [0x26] = '3',
    [0x29] = ' ',
    [0x2A] = 'v',
    [0x2B] = 'f',
    [0x2C] = 't',
    [0x2D] = 'r',
    [0x2E] = '5',
    [0x31] = 'n',
    [0x32] = 'b',
    [0x33] = 'h',
    [0x34] = 'g',
    [0x35] = 'y',
    [0x36] = '6',
    [0x3A] = 'm',
    [0x3B] = 'j',
    [0x3C] = 'u',
    [0x3D] = '7',
    [0x3E] = '8',
    [0x41] = ',',
    [0x42] = 'k',
    [0x43] = 'i',
    [0x44] = 'o',
    [0x45] = '0',
    [0x46] = '9',
    [0x49] = '.',
    [0x4A] = '/',
    [0x4B] = 'l',
    [0x4C] = ';',
    [0x4D] = 'p',
    [0x4E] = '-',
    [0x52] = 39, // '
    [0x54] = '[',
    [0x55] = '=',
    [0x5B] = ']',
    [0x5D] = '\\',
    [0x69] = '1', // numpad [0x69 -> 0x7D]
    [0x6B] = '4',
    [0x6C] = '7',
    [0x70] = '0',
    [0x71] = '.',
    [0x72] = '2',
    [0x73] = '5',
    [0x74] = '6',
    [0x75] = '8',
    [0x79] = '+',
    [0x7A] = '3',
    [0x7B] = '-',
    [0x7C] = '*',
    [0x7D] = '9'
};

unsigned char ScancodeToAsciiTable_uppercase[256] = {
    [0x0E] = '~',
    [0x15] = 'Q',
    [0x16] = '!',
    [0x1A] = 'Z',
    [0x1B] = 'S',
    [0x1C] = 'A',
    [0x1D] = 'W',
    [0x1E] = '@',
    [0x21] = 'C',
    [0x22] = 'X',
    [0x23] = 'D',
    [0x24] = 'E',
    [0x25] = '$',
    [0x26] = '#',
    [0x29] = ' ',
    [0x2A] = 'V',
    [0x2B] = 'F',
    [0x2C] = 'T',
    [0x2D] = 'R',
    [0x2E] = '%',
    [0x31] = 'N',
    [0x32] = 'B',
    [0x33] = 'H',
    [0x34] = 'G',
    [0x35] = 'Y',
    [0x36] = '^',
    [0x3A] = 'M',
    [0x3B] = 'J',
    [0x3C] = 'U',
    [0x3D] = '&',
    [0x3E] = '*',
    [0x41] = '<',
    [0x42] = 'K',
    [0x43] = 'I',
    [0x44] = 'O',
    [0x45] = ')',
    [0x46] = '(',
    [0x49] = '>',
    [0x4A] = '?',
    [0x4B] = 'L',
    [0x4C] = ':',
    [0x4D] = 'P',
    [0x4E] = '_',
    [0x52] = '"',
    [0x54] = '{',
    [0x55] = '+',
    [0x5B] = '}',
    [0x5D] = '|',
    [0x69] = '1', // numpad [0x69 -> 0x7D]
    [0x6B] = '4',
    [0x6C] = '7',
    [0x70] = '0',
    [0x71] = '.',
    [0x72] = '2',
    [0x73] = '5',
    [0x74] = '6',
    [0x75] = '8',
    [0x79] = '+',
    [0x7A] = '3',
    [0x7B] = '-',
    [0x7C] = '*',
    [0x7D] = '9'
};

void irq1_handler(void) {
    unsigned char scancode = inb(DATA_PORT);
    
    // inserting given scancode into the queue
    int next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    if (next != buffer_tail) {
        keyboard_buffer[buffer_head] = scancode;
        buffer_head = next;
    }

    PIC_sendEOI(1);
}

unsigned char get_next_scancode() {
    if (buffer_head == buffer_tail) {
        return 0; // no key
    }

    unsigned char scancode = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;

    if (sp - keyboard_stack < KEYBOARD_BUFFER_SIZE) {
        push(sp, scancode);
    }
    else if (sp - keyboard_stack == KEYBOARD_BUFFER_SIZE - 1) {
        Empty_stack(keyboard_stack, sp);
    }                

    return scancode;
}

unsigned char get_last_scancode() {
    if (sp - keyboard_stack > 0 && sp - keyboard_stack < KEYBOARD_BUFFER_SIZE - 1) {
        return pop(sp);
    }

    return 0;
}

void KeyEvent() {
    key_code.make_code = 0;
    key_code.break_code = 0;

    unsigned char data_a = get_last_scancode(); // first scancode from the stack
    unsigned char data_b = get_last_scancode(); // second scancode

    // return the scancodes back to the stack to maintain order
    push(sp, data_b); 
    push(sp, data_a);   

    if (data_b == 0xE0) { // in case of extended code- 0xE0
        if (data_a != 0xF0) { // there isn't anything to do if it's a break code- 0xF0 concidering the given order
            key_code.make_code = data_a;
        }
    }

    if (data_b == 0xF0) { // in case of breakcode
        key_code.break_code = data_a;
    }

    if (data_b != 0xF0 && data_a != 0 && data_a != 0xE0 && data_a != 0xF0) { // we want the last *keycode* not the last scancode, i.e no prefix code like 0xF0
        key_code.make_code = data_a;  
    }

    //checking the flags
    if (key_code.make_code == 0x11) {
        flags |= FLAG_LEFT_ALT;
    }
    if (key_code.break_code == 0x11) {
        flags &= ~FLAG_LEFT_ALT;
    }
    if (key_code.make_code == 0x12) {
        flags |= FLAG_LEFT_SHIFT;
    }
    if (key_code.break_code == 0x12) {
        flags &= ~FLAG_LEFT_SHIFT;
    }
    if (key_code.make_code == 0x14) {
        flags |= FLAG_LEFT_CTRL;
    }
    if (key_code.break_code == 0x14) {
        flags &= ~FLAG_LEFT_CTRL;
    }
    if (key_code.make_code == 0x58) {
        flags ^= FLAG_CAPS_LOCK;
    }
    if (key_code.make_code == 0x59) {
        flags |= FLAG_RIGHT_SHIFT;
    }
    if (key_code.break_code == 0x59) {
        flags &= ~FLAG_RIGHT_SHIFT;
    }
    if (key_code.make_code == 0x77) {
        flags ^= FLAG_NUM_LK;
    }
    if (key_code.make_code == 0x11) {
        flags |= FLAG_RIGHT_ALT;
    }
    if (key_code.break_code == 0x11) {
        flags &= ~FLAG_RIGHT_ALT;
    }
    if (key_code.make_code == 0x14) {
        flags |= FLAG_RIGHT_CTRL;
    }
    if (key_code.break_code == 0x14) {
        flags &= ~FLAG_RIGHT_CTRL;
    }
}

unsigned char get_last_keycode() {
    KeyEvent();

    return key_code.make_code;
}

unsigned char GetStackVolume() {
    return sp - keyboard_stack;
}

unsigned char parse(unsigned char keycode) {
    if (flags & FLAG_CAPS_LOCK) {
        if (flags & FLAG_LEFT_SHIFT) {
            return ScancodeToAsciiTable_lowercase[keycode];
        }
        return ScancodeToAsciiTable_uppercase[keycode];
    }
    if (flags & FLAG_LEFT_SHIFT) {
        return ScancodeToAsciiTable_uppercase[keycode];
    }
    if (flags & FLAG_NUM_LK) {
        return ScancodeToAsciiTable_lowercase[keycode];
    }
    return ScancodeToAsciiTable_lowercase[keycode];
}