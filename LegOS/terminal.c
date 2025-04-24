/* LegOS Copyright(C) 2025 Elad Yaakobi
This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "include/VGA_driver.h"
#include "include/mathlib.h"
#include "include/shapes.h"
#include "include/fonts.h"
#include "include/stringlib.h"
#include "include/picmod.h"
#include "include/intf.h"
#include "include/in-out.h"
#include "include/gdt.h"
#include "include/lidt.h"
#include "include/ps2_driver.h"
//#include "include/pit.h"
#include "include/ps2_keyboard.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern unsigned int stack_top;

/* 
	Hahaha big logo go brrrrr 
	P.S fuck invisble strings, this shit took way too long to copy ffs. 
	Probobly would've been more effecient too to do it in one function call. Maybe. than to call it over and over again for every line

	P.S P.S the logo's size exceeds the 80x25 size limit of text mode so it's impossible to even print it as ascii lmao,
	probobly gonna make it in 13h mode.

	pspspspsps even in my smallest font the 320x200 screen in 13h is too damn small >:(
	guess I'll have to make it from scratch using stuff like y = mx, etc. could be fun.

	I just made a smaller one, I have more important stuff to do first.
*/

void print_logo()
{
	draw_string(0, 0, "4x6", "          _____            _____                    _____", vga13h_Neon_Green);
	draw_string(0, 6, "4x6", "         /\\    \\          /\\    \\                  /\\    \\", vga13h_Neon_Green);
	draw_string(0, 12, "4x6", "        /::\\____\\        /::\\    \\                /::\\    \\", vga13h_Neon_Green);
	draw_string(0, 18, "4x6", "       /:::/    /       /::::\\    \\              /::::\\    \\", vga13h_Neon_Green);
	draw_string(0, 24, "4x6", "      /:::/    /       /::::::\\    \\            /::::::\\    \\", vga13h_Neon_Green);
	draw_string(0, 30, "4x6", "     /:::/    /       /:::/\\:::\\    \\          /:::/\\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 36, "4x6", "    /:::/    /       /:::/__\\:::\\    \\        /:::/  \\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 42, "4x6", "   /:::/    /       /::::\\   \\:::\\    \\      /:::/    \\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 48, "4x6", "  /:::/    /       /::::::\\   \\:::\\    \\    /:::/    / \\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 54, "4x6", " /:::/    /       /:::/\\:::\\   \\:::\\    \\  /:::/    /   \\:::\\ ___\\", vga13h_Neon_Green);
	draw_string(0, 60, "4x6", "/:::/____/       /:::/__\\:::\\   \\:::\\____\\/:::/____/  ___\\:::|    |", vga13h_Neon_Green);
	draw_string(0, 66, "4x6", "\\:::\\    \\       \\:::\\   \\:::\\   \\::/    /\\:::\\    \\ /\\  /:::|____|", vga13h_Neon_Green);
	draw_string(0, 72, "4x6", " \\:::\\    \\       \\:::\\   \\:::\\   \\/____/  \\:::\\    /::\\ \\::/    / ", vga13h_Neon_Green);
	draw_string(0, 78, "4x6", "  \\:::\\    \\       \\:::\\   \\:::\\    \\       \\:::\\   \\:::\\ \\/____/  ", vga13h_Neon_Green);
	draw_string(0, 84, "4x6", "   \\:::\\    \\       \\:::\\   \\:::\\____\\       \\:::\\   \\:::\\____\\", vga13h_Neon_Green);
	draw_string(0, 90, "4x6", "    \\:::\\    \\       \\:::\\   \\::/    /        \\:::\\  /:::/    /", vga13h_Neon_Green);
	draw_string(0, 96, "4x6", "     \\:::\\    \\       \\:::\\   \\/____/          \\:::\\/:::/    /", vga13h_Neon_Green);
	draw_string(0, 102, "4x6", "      \\:::\\    \\       \\:::\\    \\               \\::::::/    /", vga13h_Neon_Green);
	draw_string(0, 108, "4x6", "       \\:::\\____\\       \\:::\\____\\               \\::::/    /", vga13h_Neon_Green);
	draw_string(0, 114, "4x6", "        \\::/    /        \\::/    /                \\::/____/", vga13h_Neon_Green);
	draw_string(0, 120, "4x6", "         \\/____/          \\/____/", vga13h_Neon_Green);
	draw_string(0, 126, "4x6", "", vga13h_Neon_Green);
	draw_string(0, 132, "4x6", "", vga13h_Neon_Green);
	draw_string(0, 138, "4x6", "        /::\\    \\                 /\\    \\", vga13h_Neon_Green);
	draw_string(0, 142, "4x6", "       /::::\\    \\               /::\\    \\", vga13h_Neon_Green);
	draw_string(0, 150, "4x6", "      /::::::\\    \\             /::::\\    \\", vga13h_Neon_Green);
	draw_string(0, 156, "4x6", "     /::::::::\\    \\           /::::::\\    \\", vga13h_Neon_Green);
	draw_string(0, 162, "4x6", "    /:::/~~\\:::\\    \\         /:::/\\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 168, "4x6", "   /:::/    \\:::\\    \\       /:::/__\\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 174, "4x6", "  /:::/    / \\:::\\    \\      \\:::\\   \\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 180, "4x6", " /:::/____/   \\:::\\____\\   ___\\:::\\   \\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 186, "4x6", "|:::|    |     |:::|    | /\\   \\:::\\   \\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 192, "4x6", "|:::|____|     |:::|    |/::\\   \\:::\\   \\:::\\____\\", vga13h_Neon_Green);
	draw_string(0, 198, "4x6", " \\:::\\    \\   /:::/    / \\:::\\   \\:::\\   \\::/    /", vga13h_Neon_Green);
	draw_string(0, 204, "4x6", "  \\:::\\    \\ /:::/    /   \\:::\\   \\:::\\   \\/____/", vga13h_Neon_Green);
	draw_string(0, 210, "4x6", "   \\:::\\    /:::/    /     \\:::\\   \\:::\\    \\", vga13h_Neon_Green);
	draw_string(0, 216, "4x6", "    \\:::\\__/:::/    /       \\:::\\   \\:::\\____\\", vga13h_Neon_Green);
	draw_string(0, 222, "4x6", "     \\::::::::/    /         \\:::\\  /:::/    /", vga13h_Neon_Green);
	draw_string(0, 228, "4x6", "      \\::::::/    /           \\:::\\/:::/    /", vga13h_Neon_Green);
	draw_string(0, 234, "4x6", "       \\::::/    /             \\::::::/    /", vga13h_Neon_Green);
	draw_string(0, 240, "4x6", "        \\::/____/               \\::::/    /", vga13h_Neon_Green);
	draw_string(0, 246, "4x6", "         ~~                      \\::/    /", vga13h_Neon_Green);
	draw_string(0, 252, "4x6", "                                  \\/____/", vga13h_Neon_Green);
}

void print_logoALT() {
	// draw_string(0, 0, "4x6", "______________________________________________________", vga13h_Neon_Green); 	
	draw_string(0, 0 + 7, "4x6", "`7MMF'                           .g8[[8q.    .M]]bgd"	 , vga13h_Neon_Green);
	draw_string(0, 7 + 7, "4x6", "  MM                           .dP'    `YM. ,MI    ]Y" , vga13h_Neon_Green);
	draw_string(0, 14+ 7, "4x6", "  MM         .gP]Ya   .P]Ybmmm dM'      `MM `MMb."	 , vga13h_Neon_Green);
	draw_string(0, 21+ 7, "4x6", "  MM        ,M'   Yb :MI  I8   MM        MM   `YMMNq." , vga13h_Neon_Green);
	draw_string(0, 28+ 7, "4x6", "  MM      , 8M]]]]]]  WmmmP]   MM.      ,MP .     `MM" , vga13h_Neon_Green);
	draw_string(0, 35+ 7, "4x6", "  MM     ,M YM.    , 8M        `Mb.    ,dP' Mb     dM" , vga13h_Neon_Green);
	draw_string(0, 42+ 7, "4x6", ".JMMmmmmMMM  `Mbmmd'  YMMMMMb    `[bmmd]'   P]Ybmmd]"	 , vga13h_Neon_Green); 	
	draw_string(0, 48+ 7, "4x6", "_____________________6'     dP________________________", vga13h_Neon_Green); 	
	draw_string(0, 54+ 7, "4x6", "_____________________Ybmmmd'__________________________", vga13h_Neon_Green); 	
}


void clear_screen() { // technically doesn't really clear the screen so much as covering it in black.
	for (int x = 0; x < 320; x++)
	{
		for (int y = 0; y < 200; y++)
		{
			draw_pixel(x, y, 0x00);
		}
		
	}
	
}  



void pixel_art() {
	for (int i = 0; i < 3; i++)
	{
		draw_rectangle(0, i * 66, 320, 66, i * 0x4 + 2);
	}
	
	fill_circle(100, 100, 100, 0x1);
	fill_circle(50, 50, 10, 0x2);
	fill_circle(10, 25, 16, 0x3);
	fill_circle(25, 12, 4, 0x4);
	draw_circle(5, 12, 18, 0x5);
	draw_circle(100, 150, 10, 0x6);
	fill_circle(300, 200, 10, 0x7);

	for (int i = 1; i < 10; i++)
	{
		draw_circle(100, 100, i * 10, i);
	}

	x_line(20, 0x0);
	y_line(20, 0x0);	
	
}

/* 
	Display all 256 colors, although, when initialzing 13h only 64 colors are available. to get all 256 I would need to configure them myself.
*	*Done*, although, each rgb value of each of the 256 available colors can only be smaller or equal to 63, ergo it's worthless and I now need to implement SVGA...
*/
void palate_display() {
	unsigned short color = 0;
	int width = 20;
	int height = 12.5;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{	
			draw_rectangle(x * width, y * height, width, height, color);
			color++;
		}
		
	}
	
}

void Duo() {
	unsigned short c1 = vga13h_Green3; 
	unsigned short c2 = vga13h_Red_Dit;
	unsigned short c3 = vga13h_Yellow;
	unsigned short c4 = vga13h_White; 
	unsigned short c5 = vga13h_Light_Green; 

	//right foot
	fill_circle(77, 190, 10, c2);
 	fill_circle(87, 190, 10, c2); 
 	draw_rectangle(77, 180, 10, 10, c2);

	//left foot
 	fill_circle(137, 190, 10, c2);
 	fill_circle(127, 190, 10, c2); 
 	draw_rectangle(127, 180, 10, 10, c2);

	//body
 	fill_circle(107, 145, 50, c1);
	fill_circle(107, 170, 5, c5);
	draw_rectangle(102, 165, 11, 6, c1);
	fill_circle(117, 160, 5, c5);
	draw_rectangle(112, 155, 11, 6, c1);
	fill_circle(97, 160, 5, c5);
	draw_rectangle(92, 155, 11, 6, c1);

	//head
	draw_rectangle(57, 95, 101, 50, c1);
	fill_circle(69, 95, 12, c1);
	fill_circle(145, 95, 12, c1);
	draw_rectangle(69, 83, 76, 12, c1);
	fill_ellipse(35, 15, 107, 80, 0);

	//face
	

}

void font_test() {
	int offset = 0;
	for (char i = 'A'; i <= 'Z'; i++)
	{
		draw_char(10 + offset, 10, "4x6", i, vga13h_White);
		offset += 5;
	}

	offset = 0;
	for (char i = 'A'; i <= 'Z'; i++)
	{
		draw_char(10 + offset, 30, "5x12", i, vga13h_White);
		offset += 6;
	}

	offset = 0;
	for (char i = 'A'; i <= 'Z'; i++)
	{
		draw_char(10 + offset, 45, "6x8", i, vga13h_White);
		offset += 7;
	}

	offset = 0;
	for (char i = 'A'; i <= 'Z'; i++)
	{
		draw_char(10 + offset, 55, "7x12", i, vga13h_White);
		offset += 8;
	}

	offset = 0;
	for (char i = 'A'; i <= 'Z'; i++)
	{
		draw_char(10 + offset, 70, "8x12", i, vga13h_White);
		offset += 9;
	}

	offset = 0;
	for (char i = 'A'; i <= 'Z'; i++)
	{
		draw_char(10 + offset, 85, "10x18", i, vga13h_White);
		offset += 11;
	}
	
	offset = 0;
	for (char i = 'A'; i <= 'Z'; i++)
	{
		draw_char(10 + offset, 110, "12x16", i, vga13h_White);
		offset += 13;
	}


}




void Error_screen() {
	unsigned char color[9] = {vga13h_White, vga13h_Yellow1, vga13h_Cyan1, vga13h_Green1, vga13h_Purple, vga13h_Red1, vga13h_Blue, vga13h_Pink_Panther, vga13h_Neon_Green};
	unsigned char index = 0;
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 40; x++)
		{
			draw_rectangle(x * 8, y * 4, 20, 10, color[index % 9]);
			index++;
		}
		
	}
	index = 232;
	int width = 20;
	int height = 12.5;
	for (int y = 14; y < 16; y++)
	{
		for (int x = 8; x < 16; x++)
		{	
			draw_rectangle(x * width, y * height, width, height, index);
			index++;
		}
		
	}
	for (int y = 15; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{	
			draw_rectangle(x * width, y * height, width, height, index);
			index++;
		}
		
	}

	draw_rectangle(64, 40, 192, 120, 232);
	draw_rectangle(70, 46, 180, 108, 234);
	
	//E
	draw_rectangle(80, 56, 8, 88, vga13h_White);
	draw_rectangle(80, 56, 44, 8, vga13h_White);
	draw_rectangle(80, 100, 44, 8, vga13h_White);
	draw_rectangle(80, 144, 44, 8, vga13h_White);

	//rror <I'm lazy shut up>
	draw_string(130, 138, "12x16", " R R O R", vga13h_White);
	
}

void Cafe_wall() {
	unsigned char color[2] = {0, 15};
	unsigned char index = 0;
	unsigned char offset = 0;
	for (int y = 0; y < 14; y++)
	{
		for (int x = 0; x < 22; x++)
		{
			draw_rectangle(x * 14 + (offset % 2) * 4, y * 14 + 1, 14, 14 ,color[index % 2]);
			index++;
			y_line(y * 14 + 1, 236);
		}	
		offset++;
	}	
}



GdtDescriptor_t gdt_descriptor __attribute__((aligned(8)));

TSS tss1;
TSS tss2;

extern void reload_CS(void);
extern void setGdt(int limit, int base);

void prerequisite() {
	gdt_descriptor.size = (0x30 + 8) - 1; // Total size of GDT in bytes minus 1
	gdt_descriptor.base = 0x1111;         // This must match where you're writing entries

	setGdt(0, 0);
	//* GDT entries:
	//Null Descriptor is never referenced by the processor, and should always contain no data.
	unsigned char null_offset = 0x0000; //GDT entries have a pointer called offset that tells its address in memory 
	GDT nulldescipt = {
	    0,
	    0x00000000,
	    0x00,
	    0x0,
	};

	//code segment
	unsigned char kcs_offset = 0x0008;
	GDT KernelMode_CS = {
	    0,
	    0xFFFFF,
	    0x9A,
	    0xC,
	};

	//data segment
	unsigned char kds_offset = 0x0010;
	GDT KernelMode_DS = {
	    0,
	    0xFFFFF,
	    0x92,
	    0xC,
	};

	unsigned char ucs_offset = 0x0018;
	GDT UserMode_CS = {
	    0,
	    0xFFFFF,
	    0xFA,
	    0xC,
	};

	unsigned char uds_offset = 0x0020;
	GDT UserMode_DS = {
	    0,
	    0xFFFFF,
	    0xF2,
	    0xC,
	};

	//setting up TSS s
	tss1.ss0 = kds_offset;
	tss1.esp0 = stack_top;
	tss1.iopb = sizeof(tss1);

	tss2.ss0 = kds_offset;
	tss2.esp0 = stack_top;
	tss2.iopb = sizeof(tss2);
	
	//TSS gdt entries
	unsigned char tss1_offset = 0x28;
	GDT TSS1_entr = {
		(unsigned int)&tss1,
		sizeof(tss1) - 1,
		0x89,
		0x0
	}; 

	unsigned char tss2_offset = 0x30;
	GDT TSS2_entr = {
		(unsigned int)&tss2,
		sizeof(tss2) - 1,
		0x89,
		0x0
	}; 

	//the logical address is needed to tell the cpu where each entry is, it is made by the segment index (offset: 0x008, 0x010, etc) and the offset to the memory location that we decide.
	//"what should said offset be?"
	//"1 1 1 uhhhh 1!" => 0x1111

	//encoding GDT entries
	encodeGdtEntry((unsigned char *)(0x1111 + null_offset), nulldescipt);
	encodeGdtEntry((unsigned char *)(0x1111 + kcs_offset), KernelMode_CS);
	encodeGdtEntry((unsigned char *)(0x1111 + kds_offset), KernelMode_DS);
	encodeGdtEntry((unsigned char *)(0x1111 + ucs_offset), UserMode_CS);
	encodeGdtEntry((unsigned char *)(0x1111 + uds_offset), UserMode_DS);
	encodeGdtEntry((unsigned char *)(0x1111 + tss1_offset), TSS1_entr);
	encodeGdtEntry((unsigned char *)(0x1111 + tss2_offset), TSS2_entr);
	reloadSegments(&gdt_descriptor);
	

	//initialzing the interrupt descriptor table
	idt_init(); //intterupts are now enabled!

	//remapping the PIC
	PIC_remap(0x20, 0x28);

}

void kernel_main(void) 
{
	//setup
	prerequisite();
	ps2_init();
	PIT_SetHz(100);
	
	/* Initialize terminal interface */
	//set_vga_text_mode_2();
	//terminal_initialize();
	//
	//terminal_writestring("--------------------------------------------------------------------------------\n");
	//terminal_writestring("															 Hello, World!\n");
	//terminal_writestring("														Welcome to the LegOS :)\n");
	//terminal_writestring("--------------------------------------------------------------------------------\n");
	//terminal_writestring("test A\r");
	//terminal_writestring("test B\n");
	
	// testing vga mode 13h: so far switches from text mode successfully and displaying pixels correctly
	set_vga_graphics_mode_2();
	clear_screen();
	//palate_display();
	//clear_screen();
	//sleep(500);
	print_logoALT();
	//timer(10);
	//draw_string(10, 150, "7x12", ToString(stack_top, " ", 10), vga13h_White);
	//draw_rectangle(51, 51, 25, 25, 0x2);
	//pixel_art();
	//Duo();
	//font_test();
	//draw_string(60, 100, "7x12", "a", vga13h_bright_red);
	//clear_screen();


	while (1)
	{
		unsigned char keycode = get_next_keycode();
		if (keycode != 0) {
			draw_string(10, 130, "12x16", ToString(keycode, "", 10), vga13h_White);
		}
	}

}

/** 
*! FIX: need to safely switch between text mode and mode 13h, possible fixes include implementing interrupts and/or improving the set up functions 
** actually nevermind 13h rules and text mode drools; figured out fonts by myself and I have way more freedom and control than ever before 
*  TODO: (in 13h) initialize the 256 colours instead of 64, figuring out fonts(*DONE*), and desmos things.
*  TODO: figure out scrolling.
*  TODO: initialize interrupts to be availabe in protected mode to include for stuff like other drivers (i.e IDT, ISR, PIC for VGA, Keyboard, etc) *DONE* (still need to figure out handers tho)
*/
