#ifndef IN_OUT_H
#define IN_OUT_H

#include <stdint.h>
#include <stddef.h>

//sends a 8/16/32 bit value from an I/O location
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

//receives a 8/16/32 bit value from an I/O location
static inline uint8_t inb(uint16_t port) 
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
    return ret;
}

//Wait a very small amount of time (1 to 4 microseconds, generally). Useful for implementing a small delay for PIC remapping.
static inline void io_wait(void)
{
    outb(0x80, 0);
}


#endif // in-out.h
