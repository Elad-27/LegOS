#ifndef LIDT_H
#define LIDT_H

void idt_set_descriptor(unsigned char vector, void* isr, unsigned char flags);
void idt_init(void);

//structure of a single (32-bit) IDT entry:
typedef struct {
	unsigned short    isr_low;      // The lower 16 bits of the ISR's address
	unsigned short    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	unsigned char     reserved;     // Set to zero
	unsigned char     attributes;   // Type and attributes; see the IDT page
	unsigned short    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

//structure for a 32-bit idtr
typedef struct {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)) idtr_t;



#endif