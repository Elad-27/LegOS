#ifndef PS2_DRIVER_H
#define PS2_DRIVER_H

#include <stdint.h>
#include <stddef.h>
#include "in-out.h" 
#include "picmod.h"
#include <stdbool.h>


// ps/2 controller ports:
/*     |Purpose             |IO Port |Access Type*/
#define DATA_PORT            0x60   //Read/Write
#define STATUS_REGISTER      0x64   //Read
#define COMMAND_REGISTER     0x64   //Write

/**
 * !info: 
 * The PS/2 Controller itself uses 2 IO ports (IO ports 0x60 and 0x64). Like many IO ports, reads and writes may access different 
 * internal registers. 
 * 
 **Data Port:
 * The Data Port (IO Port 0x60) is used for reading data that was received from a PS/2 device or from the PS/2 controller itself 
 * and writing data to a PS/2 device or to the PS/2 controller itself. 
 * 
 **Status Register: 
 * The Status Register contains various flags that show the state of the PS/2 controller. The meanings for each bit are:
 *  
 *!Bit: | Meaning:
 ** 0   | Output buffer status (0 = empty, 1 = full) (must be set before attempting to read data from IO port 0x60) 
 ** 1   | Input buffer status (0 = empty, 1 = full) (must be clear before attempting to write data to IO port 0x60 or IO port 0x64) 
 ** 2   | System Flag, Meant to be cleared on reset and set by firmware (via. PS/2 Controller Configuration Byte) if the system passes self tests (POST) 
 ** 3   | Command/data (0 = data written to input buffer is data for PS/2 device, 1 = data written to input buffer is data for PS/2 controller command) 
 ** 4   | Unknown (chipset specific) May be "keyboard lock" (more likely unused on modern systems) 
 ** 5   | Unknown (chipset specific) May be "receive time-out" or "second PS/2 port output buffer full" 
 ** 6   | Time-out error (0 = no error, 1 = time-out error) 
 ** 7   | Parity error (0 = no error, 1 = parity error) 
 *
 **Command Register:
 * The Command Port (IO Port 0x64) is used for sending commands to the PS/2 Controller (not to PS/2 devices). 
 ** The PS/2 Controller accepts commands and performs them. These commands should not be confused with bytes sent to a PS/2 device (e.g. keyboard, mouse). 
 *
 * *Connectors: 
 *  The first ps/2 connector connects to the keyboard, it uses the ps/2 controller to communicate with the cpu through IRQ1 and IRQ2
 *  The second ps/2 connector connects to the mouse, it uses the ps/2 controller to communicate with the cpu through IRQ12 that then goes through the first connector IRQ line (IRQ1 and IRQ2)
*/



//prototypes:
int ps2_init();
void Output_buffer_status();
void Input_buffer_status();
int send_port1(unsigned char data);
int send_port2(unsigned char data);
//for debugging purposes:
void step3();
void step4();
void step5();
int step6();
void step7();
int step8();
void step9();
void step10();


#endif