#include "../../include/pit.h"



void PIT_SetHz(unsigned int freq) {
    unsigned short divisor = PIT_FREQUENCY / freq;

    // Send command byte
    outb(PIT_COMMAND, 0x36);

    // Send frequency divisor
    outb(PIT_CHANNEL0, divisor & 0xFF);        // Low byte
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF); // High byte
}

void timer_callback() {
    timer_ticks++;
}

void sleep(unsigned int ticks) {
    unsigned int target = timer_ticks + ticks;
    while (timer_ticks < target) {

    }
}

void timer(int seconds) {
	int counter = 0000000;
	char* result;
	draw_string(160, 100, "8x12", ToString(counter, result, 10), vga13h_Red);

	while(counter <= seconds * 100) {
		draw_rectangle(160, 100, 60, 14, vga13h_Black);
		draw_string(160, 100, "8x12", ToString(counter / 100, result, 10), vga13h_Red);
        sleep(10);
		counter += 10;
	}
} 

