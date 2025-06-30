#ifndef ISR_H
#define ISR_H

#include "stringlib.h"
#include "shapes.h"
#include "VGA_driver.h"


//general exception handler:
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
//currently it's an awful but usful placeholder


#endif