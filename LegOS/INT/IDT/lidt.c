#include "../../include/lidt.h"
#include <stdbool.h>


__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr;

//helper function to define entries in the IDT
void idt_set_descriptor(unsigned char vector, void* isr, unsigned char flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (unsigned int)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (unsigned int)isr >> 16;
    descriptor->reserved       = 0;
}

static bool vectors[48];
extern void* isr_stub_table[];
extern void* irq_stub_table[];
void idt_init() {
    idtr.base = (unsigned int)&idt[0];
    idtr.limit = (unsigned short)sizeof(idt_entry_t) * 48 - 1;

    for (unsigned char vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    for (unsigned char vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector + 32, irq_stub_table[vector], 0x8E);
        vectors[vector + 32] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}