#include "../../include/picmod.h"
#include "../../include/in-out.h"


//common set of definitions:
#define PIC1            0x20 //IO base address for master PIC
#define PIC2            0xA0 //IO base for slave PIC
#define PIC1_COMMAND    PIC1
#define PIC1_DATA   (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA   (PIC2+1)

#define PIC_EOI         0x20 // End-of-interrupt command code

/*
* One of the most common issued command to the PIC chips - end of interrupt(EOI) command (code 0x20).
* This is issued to the PIC chips at the end of an IRQ-based interrupt routine. If the PIC came from the Master PIC,
* it is sufficient to issue this command only to the Master PIC; however, if the IRQ came from the slave PIC, it is necessary
* to issue the command to both PIC chips
*/ 
void PIC_sendEOI(uint8_t irq)
{
    if(irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

//Initialisation:
/*
when entering protected mode the first command needed to give the two PICs is the initialise command (code 0x11).
this command makes the PIC wait for three extra "initialisation words" on the data port.
these bytes give the PIC:
* Its own vector offset (ICW2),
* Tell it how it is wired to master/slaves (ICW3),
* Gives additional information about the environment (ICW4).
*/

/* reinitialisize the PIC controllers, giving them specified vector offsets, rather than 8h and 70h, as configured by default*/
#define ICW1_ICW4       0x01 // Indicates that ICW4 wil be present
#define ICW1_single     0x02 // Single (cascade) mode
#define ICW1_INTERVAL4  0x04 // Call address interval 4 (8)
#define ICw1_level      0x08 // Level triggered (edge) mode
#define ICW1_INIT       0x10 // Initialization - required!

#define ICW4_8086       0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02 // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08 // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_SFNM       0x10 // Special fully nested (not)

/*
arguments:
*   offset1 - vector for master PIC
    vectors on the master become offset1 -> offset1+7
*   offset2 - same for slave PIC: 
    offset2 -> offset2+7
*/

void PIC_remap(int offset1, int offset2)
{
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); //starts the initialization sequence (in cascade mode)
    io_wait();                                   
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1);                  //ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2);                  //ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4);                        //ICW3: tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);                        //ICW3: tell slave PIC its cascade identity (0000 0010)
    io_wait();


    outb(PIC1_DATA, ICW4_8086);                //ICW4: have the PICs use 8086 mode (and not 8080 mode)
    io_wait();
    outb(PIC2_DATA, ICW4_8086);

    //Unmask both PICs
    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);

    unsigned char mask = inb(0x21);
    mask &= ~(1 << 1);
    outb(0x21, mask);
}
//Note the presence of io_wait() calls, on older hardware & machines it's mecessary to give the PIC some time to react to commands
// as they might not be processed quickly