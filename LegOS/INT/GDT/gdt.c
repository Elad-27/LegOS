#include "../../include/gdt.h"

void encodeGdtEntry(unsigned char *target, struct GDT source)
{
    // Check the limit to make sure that it can be encoded
    if (source.limit <= 0xFFFFF) {
    
        // Encode the limit
        target[0] = source.limit & 0xFF;
        target[1] = (source.limit >> 8) & 0xFF;
        target[6] = (source.limit >> 16) & 0x0F;
        
        // Encode the base
        target[2] = source.base & 0xFF;
        target[3] = (source.base >> 8) & 0xFF;
        target[4] = (source.base >> 16) & 0xFF;
        target[7] = (source.base >> 24) & 0xFF;
        
        // Encode the access byte
        target[5] = source.access_byte;
        
        // Encode the flags
        target[6] |= (source.flags << 4);
    }
}

/*
void tss_setup(TSS tss, unsigned short syscall_val) {
    tss.ss0 = kds_offset; // kernel data segment discriptor
    tss.esp0 = syscall_val;// the value the stack-pointer shall get at a system call
    tss.iopb = sizeof(tss);
}
*/

/*
void GDT_tss_mkentr(TSS tss, unsigned char offset) {
    
    GDT tss_entry;
    unsigned char tss_offset = offset;
    tss_entry.base = &tss;
    tss_entry.limit = sizeof(tss) - 1;
    tss_entry.access_byte = 0x89;
    tss_entry.flags = 0x0;
}
*/