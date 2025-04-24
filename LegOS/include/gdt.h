#ifndef GDT_H
#define GDT_H

// Segment_Selector info:
/** |  bit   | def 
**  | 1 - 0  | The requested Privilege Level of the selector, determines if the selector is valid during permission checks and may set execution or memory access privilege.
**  | 2      | Specifies which descriptor table to use. If clear (0) then the GDT is used, if set (1) then the current LDT is used.
**  | 3 - 15 | Bits 3-15 of the Index of the GDT or LDT entry referenced by the selector. Since Segment Descriptors are 8 bytes in length, the value of Index is never unaligned and contains all zeros in the lowest 3 bits.
*/

typedef struct GDT
{
    int base;
    int limit;
    int access_byte;
    int flags;
}GDT;

typedef struct GdtDescriptor {
    unsigned short size;
    unsigned int base;
} __attribute__((packed)) GdtDescriptor_t;

//prototypes:
void encodeGdtEntry(unsigned char *target, struct GDT source);
extern void reloadSegments(void* gdt_descriptor_ptr);



// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR
 
#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// The Task State Segment, holds information about a task. In Protected Mode the TSS is primarily suited for Hardware Task Switching, where each individual Task has its own TSS. 
typedef struct TSS
{
    unsigned short link; // Previous Task Link Field. Contains the Segment Selector for the TSS of the previous task.
    unsigned short ss0; 
    unsigned short ss1; // The Segment Selectors used to load the stack when a privilege level change occurs from a lower privilege level to a higher one.
    unsigned short ss2;
    unsigned short esp0; 
    unsigned short esp1; // The Stack Pointers used to load the stack when a privilege level change occurs from a lower privilege level to a higher one.
    unsigned short esp2;
    unsigned short iopb; // I/O Map Base Address Field. Contains a 16-bit offset from the base of the TSS to the I/O Permission Bit Map.
    unsigned short ssp; // Shadow Stack Pointer, basically a return address. 
//general purpose registers and more:
    unsigned short ldtr;
    unsigned short gs;
    unsigned short fs;
    unsigned short ds;
    unsigned short ss;
    unsigned short cs;
    unsigned short es;
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    unsigned int eflags; //flags register
    unsigned int eip; // instruction pointer
    unsigned int cr3; // control register 3
}TSS;

/** TSS in setup:
 ** members to be set:
 * SS0 gets the kernel datasegment descriptor (e.g. 0x10 if the third entry in your GDT describes your kernel's data)
 * ESP0 gets the value the stack-pointer shall get at a system call
 * IOPB may get the value sizeof(TSS)
 * 
 ** loading of the TSS must take place in protected mode and after the GDT has been loaded.  
 */

//For use in software multitasking, one or two TSS s are also generally used, as they allow for entering Ring 0 code after an interrupt.

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


#endif