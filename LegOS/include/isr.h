#ifndef ISR_H
#define ISR_H

#include "stringlib.h"
#include "shapes.h"
#include "VGA_driver.h"


//general exception handler:
__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
//currently it's an awful but usful placeholder


#endif