#ifndef INTF_H
#define INTF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


//Push / Pop interrupt flag
static inline unsigned long save_irqdisable(void)
{
    unsigned long flags;
    asm volatile ("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
    return flags;
}
//Sometimes it is helpful to disable interrupts and then only re-enable them if they were disabed before.

//are interrupts enabled?
//returns a true boolean value if IRQs are enabled for the CPU.
static inline bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

static inline void INT() {
    __asm__ __volatile__ ("int $0x21");
}

#endif