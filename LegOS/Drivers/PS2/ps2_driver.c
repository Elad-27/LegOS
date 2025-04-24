#include "../../include/ps2_driver.h"  

/** Usful knowledge:
 *  Ascii chars: each visable character on the keyboard has its own identifying ascii value- 'A' = 65, ' ' = 32, etc
 ** this only applies to readable characters like letters, numbers, and special chars <>,/?';!. keys like shift, tab, or enter don't have an ascii value.
 *  This is because ascii is intended to convert between and represent chars on the screen from keys on the keyboard
 * 
 * Scancodes: these are codes sent from the keyboard itself to the computer telling it what key was pressed and where, this applies to all keys on the keyboard
 **To communicate what key was just pressed and what key was just released. the scancodes are a collection of the location of all the keys on the keyboard,
 * put together forms a map that is represents something closer to "a2 square" on a 8x8 chessboard; it doesn't tell what letter or number / pawn or rook is on the a2 square.    
 *
 * Keycodes: after the driver interpets and analyizes the scancodes it determines what specific character was typed by the user:
 **"the scancode for 'a' was pressed!", "Is the shift pressed?" *yes*,  therefor the typed char is 'A'! / "the scancode for '4' was pressed!", "Is the num lk active?" *no*,  therefor the typed char is left arrow! move one space to the left!
 * and returns a corresponding code to either print or preform an action. 
 * 
 * ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 
 * ! PS/2 Controller:
 * * commmand - response process:
 *   first you send a command byte (e.g 0x20) to the command register (0x64), 
 *   if said command also gives a response, the response needs to be read from the data port (0x60). 
 * * Process example: 
 *   outb(COMMAND_REGISTER, 0x20); - sends the 0x20 command byte to the command register(Read "byte 0" from internal RAM), 
 *   inb(DATA_PORT); - read the response from the controller (Controller Configuration Byte)
 * 
 * * Clearing and setting bits:
 *   config |= (1 << bit); -setting bit
 *   config &= ~(1 << bit) -clearing bit
 * * where config is an unsigned char that represents the configure byte
 *   and bit is the bit you want to change (e.g bit 0, bit 6, etc) 
 */

//Initialising the PS/2 Controller:
/**
 * *step 1: Initialise (then disable) USB Controllers 
 *  to ensure they won't interfere with the ps/2 controller
 * 
 * *step 2: Determine if the PS/2 Controller Exists
 * The correct way to do this is is with ACPI. 
 * Check bit 1 (value = 2, the "8042" flag) in the "IA PC Boot Architecture Flags" field at offset 109 in the Fixed ACPI Description Table (FADT). 
 * If this bit is clear, then there is no PS/2 Controller to configure. Otherwise, if the bit is set, or the system doesn't support ACPI (no ACPI tables and no FADT) then there is a PS/2 Controller. 
 * TODO: set up the ACPI apperantly
 * 
 * *step 3: Disable Devices
 * Devices connected to the PS/2 port(s) must be disabled so that they won't send data at the wrong time and mess up your initialisation; 
 * start by sending a command 0xAD and command 0xA7 to the PS/2 controller. If the controller is a single channel device, it will ignore the command 0xA7. 
 * 
 * *Step 4: Flush The Output Buffer
 * Sometimes data can get stuck in the PS/2 controller's output buffer, disabling the devices and clearing the output buffer makes errors alot less likely
 * 
 * *Step 5: Set the Controller Configuration Byte
 *
 * *Step 6: Perform Controller Self Test
 * To test the PS/2 controller, send command 0xAA to it, then wait for it to respond and check if it replied with 0x55 
 * (any value other than 0x55 indicates a self-test fail)
 * 
 * *Step 7: Determine If There Are 2 Channels 
 * if the 2nd port exists, then the ps/2 controller and the OS can use the mouse, 
 * otherwise, the port for the mouse simply doesn't exist for some reason (might be hardware specific on newer machines or something)
 * 
 * *Step 8: Perform Interface Tests
 * This step tests the PS/2 ports. Use command 0xAB to test the first PS/2 port, then check the result. (0x00 = test passed)
 * Then (if it's a dual channel controller) use command 0xA9 to test the second PS/2 port, then check the result. (0x00 = test passed)
 * 
 * *Step 9: Enable Devices
 * Enable any PS/2 port that exists and works using command 0xAE (for the first port) and command 0xA8 (for the second port). 
 * If you're using IRQs (recommended), also enable interrupts for any (usable) PS/2 ports in the Controller Configuration Byte (set bit 0 for the first PS/2 port, and/or bit 1 for the second PS/2 port, then set it with command 0x60). 
 * 
 * *Step 10: Reset Devices
 * 
*/

/**
 * !ok so emulating LegOS using QEMU ensures with 100% certianty that not only the USB support is offline by defualt but also that the ps/2 controller and hardware are present and active by default. 
 * *Ergo: steps 1 to 2 are irrelevent when emulating with QEMU (which is good since I have no idea how to implement them). 
 * *However, it means that running this OS on actual hardware would be unsafe and unoptimized, 2 core tenets of programming I try to abide by, therefor, I will try to either figure out these 2 steps regardless or find a better implementation method for actual hardware *eventually*.
 */

//must be set before attempting to read data from IO port 0x60
void Output_buffer_status() {
    while(!(inb(STATUS_REGISTER) & (1 << 0))) {
        io_wait();
    }
}

//must be clear before attempting to write data to IO port 0x60 or IO port 0x64
void Input_buffer_status() {
    while(inb(STATUS_REGISTER) & (1 << 1)) {
        io_wait();
    }
}

//sending bytes to device/s
//port1:
int send_port1(unsigned char data) {
    int timer = 1000000;
    while(inb(STATUS_REGISTER) & (1 << 1)) {
        timer--;
        if (timer == 0) {
            return 1;
        }
    }

    outb(DATA_PORT, data);

    return 0;
}

//port2:
int send_port2(unsigned char data) {
    int timer = 1000000;
    outb(COMMAND_REGISTER, 0xD4);

    while(inb(STATUS_REGISTER) & (1 << 1)) {
        timer--;
        if (timer == 0) {
            return 1;
        }
    }

    outb(DATA_PORT, data);

    return 0;
}

unsigned char config;

bool port1;
bool port2;

bool is_keyboard_enabled;
bool does_2nd_port_exist;
bool is_mouse_enabled;

unsigned char keyboard_port; //1 for port one, 2 for port two
unsigned short keyboard_ID; // e.g 0xAB, 0x54 
unsigned char mouse_port;  //1 for port one, 2 for port two
unsigned short mouse_ID;  // e.g 0x00 

//Disabling devices:
void step3() {
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xAD);
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xA7);
    
}

//flushing output buffer
void step4 (){
    inb(DATA_PORT);    
}

//configuring bytes
void step5() {
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x20);
    Output_buffer_status();
    unsigned char config = inb(DATA_PORT);
    config &= ~(1 << 0); //clear bit 0 (First PS/2 port interrupt (1 = enabled, 0 = disabled))
    config &= ~(1 << 6); //clear bit 6 (First PS/2 port translation (1 = enabled, 0 = disabled))
    config &= ~(1 << 4); //clear bit 4 (makes sure the clock signal is enabled)
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x60); //telling the controller we want to modify the configure byte
    outb(DATA_PORT, config);
}

//Controller Self Test
int step6() {
    unsigned char bits[8] = {
    (config >> 0) & 1, (config >> 1) & 1, (config >> 2) & 1, (config >> 3) & 1, 
    (config >> 4) & 1, (config >> 5) & 1, (config >> 6) & 1, (config >> 7) & 1
    };
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xAA);
    Output_buffer_status();
    if (inb(DATA_PORT) != 0x55) {
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xAA);
        Output_buffer_status();
        if (inb(DATA_PORT) != 0x55) {
            return 1;
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (bits[i]) {
            config |= (1 << i);
        }
        else
        {
            config &= ~(1 << i);
        }
    }
    

    return 0;
}

//Dual channel check
void step7() {
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xA8);
    outb(COMMAND_REGISTER, 0x20);
    Output_buffer_status();
    config = inb(DATA_PORT);
    if((config >> 5) & 1 == 0) {
        does_2nd_port_exist = true;
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xA7);
        config &= ~(1 << 1);
        config &= ~(1 << 5);
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0x60); 
        outb(DATA_PORT, config);    
    }
    else {
        is_mouse_enabled = false;
    }
}

//Ports test
int step8() {
    //first port:
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xAB);
    Output_buffer_status();
    unsigned char result = inb(DATA_PORT);
    if (result != 0x00) {
        port1 = false;
    }
    else
    {
        port1 = true;
    }
    

    //2nd port:
    if (does_2nd_port_exist) {
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xA9);
        Output_buffer_status();
        result = inb(DATA_PORT);
        if (result != 0x00) {
            port2 = false;
        }
        else
        {
            port2 = true;
        }
    }

    if (port1 == false && port2 == false) {
        return 1; //giving up and trying again later
    }


    return 0;
}

//Enabling devices
void step9() {
    if (port1) {
        config |= (1 << 0);
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xAE);
    }

    if (port2) {
        config |= (1 << 1);
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xA8);
    }

    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x60); 
    outb(DATA_PORT, config);    
}

//resetting devices
void step10() {
    unsigned char resp_a = 0;
    unsigned char resp_b = 0;

    unsigned char ID_low = 0;
    unsigned char ID_high = 0;
    if(port1) {
        send_port1(0xFF);
        Output_buffer_status();
        resp_a = inb(DATA_PORT);
        if (resp_a != 0xFC) {
            Output_buffer_status();
            resp_b = inb(DATA_PORT);    
        }
        else
        {
            port1 = false;
        }
        
    }
    if ((resp_a == 0xFA || resp_a == 0xAA) && (resp_b == 0xFA || resp_b == 0xAA) && resp_a != resp_b) {
        send_port1(0xF5);
        Output_buffer_status();
        if (inb(DATA_PORT) == 0xFA) {
            send_port1(0xF2);
            Output_buffer_status();
            if (inb(DATA_PORT) == 0xFA) {
                Output_buffer_status();
                ID_high = inb(DATA_PORT);
                if (ID_high > 0x04) {
                    keyboard_port = 1;
                    Output_buffer_status();
                    ID_low = inb(DATA_PORT);

                    keyboard_ID = ID_high;
                    keyboard_ID << 8;
                    keyboard_ID += ID_low;
                }
                else
                {
                    mouse_port = 1;
                    mouse_ID = ID_high;
                    keyboard_port = 2;
                }
            }
        }   
    }

    resp_a = 0;
    resp_b = 0;

    ID_high = 0;
    ID_low = 0;

    if(port2) {
        send_port2(0xFF);
        Output_buffer_status();
        resp_a = inb(DATA_PORT);
        if (resp_a != 0xFC) {
            Output_buffer_status();
            resp_b = inb(DATA_PORT);    
        }
        else
        {
            port2 = false;
        }
        
    }
    if ((resp_a == 0xFA || resp_a == 0xAA) && (resp_b == 0xFA || resp_b == 0xAA) && resp_a != resp_b) {
        send_port2(0xF5);
        Output_buffer_status();
        if (inb(DATA_PORT) == 0xFA) {
            send_port2(0xF2);
            Output_buffer_status();
            if (inb(DATA_PORT) == 0xFA) {
                Output_buffer_status();
                ID_high = inb(DATA_PORT);
                if (ID_high > 0x04) {
                    keyboard_port = 2;
                    Output_buffer_status();
                    ID_low = inb(DATA_PORT);

                    keyboard_ID = ID_high;
                    keyboard_ID << 8;
                    keyboard_ID += ID_low;
                }
                else
                {
                    mouse_port = 2;
                    mouse_ID = ID_high;
                }
            }
        }   
    }
}

int ps2_init() {
    unsigned char config;

    bool port1;
    bool port2;
    
    bool is_keyboard_enabled;
    bool does_2nd_port_exist;
    bool is_mouse_enabled;
    
    unsigned char keyboard_port; //1 for port one, 2 for port two
    unsigned short keyboard_ID; // e.g 0xAB, 0x54 
    unsigned char mouse_port;  //1 for port one, 2 for port two
    unsigned short mouse_ID;  // e.g 0x00 
    

    //disabling devices:
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xAD);
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xA7);

    //flushing output buffer
    inb(DATA_PORT);

    //configuring bytes
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x20);
    Output_buffer_status();
    config = inb(DATA_PORT);
    config &= ~(1 << 0); //clear bit 0 (First PS/2 port interrupt (1 = enabled, 0 = disabled))
    config &= ~(1 << 6); //clear bit 6 (First PS/2 port translation (1 = enabled, 0 = disabled))
    config &= ~(1 << 4); //clear bit 4 (makes sure the clock signal is enabled)
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x60); //telling the controller we want to modify the configure byte
    outb(DATA_PORT, config);

    //Controller Self Test
    unsigned char bits[8] = {
        (config >> 0) & 1, (config >> 1) & 1, (config >> 2) & 1, (config >> 3) & 1, 
        (config >> 4) & 1, (config >> 5) & 1, (config >> 6) & 1, (config >> 7) & 1
    };    
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xAA);
    Output_buffer_status();
    if (inb(DATA_PORT) != 0x55) {
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xAA);
        Output_buffer_status();
        if (inb(DATA_PORT) != 0x55) {
            return 1;
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (bits[i]) {
            config |= (1 << i);
        }
        else
        {
            config &= ~(1 << i);
        }
    }

    //Dual channel check
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xA8);
    outb(COMMAND_REGISTER, 0x20);
    Output_buffer_status();
    config = inb(DATA_PORT);
    if((config >> 5) & 1 == 0) {
        does_2nd_port_exist = true;
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xA7);
        config &= ~(1 << 1);
        config &= ~(1 << 5);
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0x60); 
        outb(DATA_PORT, config);    
    }
    else {
        is_mouse_enabled = false;
    }

    //Ports test - refer to step 8 for test results
    //first port:
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0xAB);
    Output_buffer_status();
    unsigned char result = inb(DATA_PORT);
    if (result != 0x00) {
        port1 = false;
    }
    else
    {
        port1 = true;
    }
    

    //2nd port:
    if (does_2nd_port_exist) {
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xA9);
        Output_buffer_status();
        result = inb(DATA_PORT);
        if (result != 0x00) {
            port2 = false;
        }
        else
        {
            port2 = true;
        }
    }

    if (port1 == false && port2 == false) {
        return 1; //giving up and trying again later
    }
    //after testing I came to the conclusion that port2 is *NOT* available 
    //Edit: my bad, after restoring the configuration byte it was fixed: there *IS* a port2 and it is *AVAILABLE*

    //Enabling devices
    if (port1) {
        config |= (1 << 0);
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xAE);
    }

    if (port2) {
        config |= (1 << 1);
        Input_buffer_status();
        outb(COMMAND_REGISTER, 0xA8);
    }

    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x60); 
    Input_buffer_status();
    outb(DATA_PORT, config); 



    /**
    * *testing config byte to disable interrupts from the ports: 
    *  currently my IRQ lines and interrupt rutines are pieces of hot garbage, 
    *  so for now I will try to poll the ports just to see if works (before properly making ISR and interrupt handlers etc).
    */
    config &= ~(1 << 0);
    config &= ~(1 << 1);
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x60); 
    outb(DATA_PORT, config);    

    //Resetting Decives
    unsigned char resp_a = 0;
    unsigned char resp_b = 0;
    
    unsigned char ID_low = 0;
    unsigned char ID_high = 0;
    if(port1) {
        if ((inb(STATUS_REGISTER) & (1 << 1)) == 0) {
            outb(DATA_PORT, 0xFF);
        }
        Output_buffer_status();
        resp_a = inb(DATA_PORT);
        if (resp_a != 0xFC) {
            Output_buffer_status();
            resp_b = inb(DATA_PORT);    
        }
        else
        {
            port1 = false;
        }
        
    }
    if ((resp_a == 0xFA || resp_a == 0xAA) && (resp_b == 0xFA || resp_b == 0xAA) && resp_a != resp_b) {
        send_port1(0xF5);
        Output_buffer_status();
        if (inb(DATA_PORT) == 0xFA) {
            send_port1(0xF2);
            Output_buffer_status();
            if (inb(DATA_PORT) == 0xFA) {
                Output_buffer_status();
                ID_high = inb(DATA_PORT);
                if (ID_high > 0x04) {
                    keyboard_port = 1;
                    Output_buffer_status();
                    ID_low = inb(DATA_PORT);

                    keyboard_ID = ID_high;
                    keyboard_ID << 8;
                    keyboard_ID += ID_low;
                }
                else
                {
                    mouse_port = 1;
                    mouse_ID = ID_high;
                    keyboard_port = 2;
                }
            }
        }   
    }
    //keyboard_ID = 0xAB, 0x83 : MF2 keyboard 

    resp_a = 0;
    resp_b = 0;

    ID_high = 0;
    ID_low = 0;

    if(port2) {
        send_port2(0xFF);
        Output_buffer_status();
        resp_a = inb(DATA_PORT);
        if (resp_a != 0xFC) {
            Output_buffer_status();
            resp_b = inb(DATA_PORT);    
        }
        else
        {
            port2 = false;
        }
        
    }
    if ((resp_a == 0xFA || resp_a == 0xAA) && (resp_b == 0xFA || resp_b == 0xAA) && resp_a != resp_b) {
        send_port2(0xF5);
        Output_buffer_status();
        if (inb(DATA_PORT) == 0xFA) {
            send_port2(0xF2);
            Output_buffer_status();
            if (inb(DATA_PORT) == 0xFA) {
                Output_buffer_status();
                ID_high = inb(DATA_PORT);
                if (ID_high > 0x04) {
                    keyboard_port = 2;
                    Output_buffer_status();
                    ID_low = inb(DATA_PORT);

                    keyboard_ID = ID_high;
                    keyboard_ID << 8;
                    keyboard_ID += ID_low;
                }
                else
                {
                    mouse_port = 2;
                    mouse_ID = ID_high;
                }
            }
        }   
    }

    // renabling IRQs
    config |= (1 << 0);
    //config |= (1 << 1);
    Input_buffer_status();
    outb(COMMAND_REGISTER, 0x60); 
    Input_buffer_status();
    outb(DATA_PORT, config);    

    return 0;
}

/**
 * TODO: polling the ports for resetting or initialzation of the controller is alright, but we have to have a functioning IRQs handler and servicer to type stuff
 * *Due date:
 * !now bitch
 */