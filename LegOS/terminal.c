/* 		LegOS Copyright(C) 2025 Elad Yaakobi
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
#include "include/ps2_keyboard.h"
#include "include/stdleg.h"
#include "include/memleg.h"


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

	x_line(0, 200, 20, 0x0);
	y_line(0, 320, 20, 0x0);	
	
}

/* 
	Display all 256 colors, although, when initialzing 13h only 64 colors are available. to get all 256 I would need to configure them myself.
*	*Done*, although, each rgb value of each of the 256 available colors can only be smaller or equal to 63, ergo it's worthless and I now need to implement SVGA...
*/
void palate_display() {
	draw_rectangle(5, 5, 170, 170, 241);
	unsigned short color = 0;
	int width = 10;
	int height = 10;
	for (int y = 0; y < 16; y++)
	{
		y_line(10, 171, y * height + 10, 238);
		for (int x = 0; x < 16; x++)
		{	
			x_line(10, 171, x * width + 10, 238);
			draw_rectangle(x * width + 11, y * height + 11, width, height, color);
			color++;
		}
	}
	y_line(10, 171, 16 * height + 10, 238);
	x_line(10, 171, 16 * width + 10, 238);
	
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
			y_line(0, 320, y * 14 + 1, 236);
		}	
		offset++;
	}	
}

void wavy_tiles () {
	unsigned char colors[2] = {vga13h_Yellow, vga13h_Orange1};
	unsigned char index = 0;
	for (int y = 0; y < 14; y++) // square grid
	{
		for (int x = 0; x < 22; x++)
		{
			draw_rectangle(x * 14, y * 14, 14, 14, colors[index % 2]);
			index++;
		}
		index--;	
	}
	
	unsigned char colorsB[4] = {vga13h_Red_Dit, vga13h_White, vga13h_White, vga13h_Red_Dit};
	index = 0;
	for (int y = 0; y < 14; y++) // +
	{
		for (int x = 1; x < 22; x++)
		{
			draw_char(x * 14 - 2, y * 14 - 2, "4x6", '+', colorsB[index % 4]);
			index++;
		}	
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
	IRQ_set_mask(0);
	IRQ_set_mask(1);
	IRQ_set_mask(4);
	IRQ_set_mask(12);

}

void loading_screen() {
	// progress bar
	draw_rectangle(106, 120, 106, 21, vga13h_Grey420);
	fill_circle(106, 130, 10, vga13h_Grey420);
	fill_circle(213, 130, 10, vga13h_Grey420);
	
	// load bar
	for (int i = 0; i <= 107; i++)
	{
		fill_circle(106 + i, 130, 8, vga13h_White);
		sleep(Math_Sin(i % 7) * 10 - 3);
		// room to do literally anything other than just make it look like something is actually loading
	}

    unsigned char colors[6] = {238, 239, 240, 241, 240, 239};
    unsigned char index = 0;
    unsigned int target = GetTicks() + 30;
	
	get_next_scancode(); // empty buffer
	while (get_next_scancode() == 0) // text glow animation
	{
		draw_string(80, 150, "7x12", "Press any key to continue . . .", colors[index % 6]);
        if (GetTicks() >= target)
        {
			draw_string(80, 150, "7x12", "Press any key to continue . . .", colors[index % 6]);
            index++;
            target = GetTicks() + 30;
        }
		asm("hlt");
	}
	/**
	 *TODO: implement a way to make this work for the power button and car / house keys as well 
	*/

	clear_screen();
	
}

void choice() {
	// display the options
	draw_string (40, 100, "7x12", "(*) Option 1:", vga13h_Dark_Green);
	draw_string (40, 120, "7x12", "(*) Option 2:", vga13h_Dark_Green);
	draw_string (40, 140, "7x12", "(*) Option 3:", vga13h_Dark_Green);

	bool flag = false;
	unsigned char options[3] = {0, 1, 2};
	unsigned char offset = 0;
	while (!flag)
	{
		unsigned char keycode = get_next_scancode();
		unsigned char last = get_last_keycode();
		if (last == 0x0D) { // tab
			rect_out(39, 98 + options[offset % 3] * 20, 92, 14, vga13h_Dark_Orange);
			rect_out(39, 98 + options[(offset + 1) % 3] * 20, 92, 14, vga13h_Black);
			rect_out(39, 98 + options[(offset + 2) % 3] * 20, 92, 14, vga13h_Black);
			offset++;
			sleep(15);
		}
		else if (last == 0x16) // 1
		{
			rect_out(39, 98, 92, 14, vga13h_Dark_Orange);
			rect_out(39, 98 + 1 * 20, 92, 14, vga13h_Black);
			rect_out(39, 98 + 2 * 20, 92, 14, vga13h_Black);
			sleep(15);
		}
		else if (last == 0x1E) // 2
		{
			rect_out(39, 98 + 1 * 20, 92, 14, vga13h_Dark_Orange);
			rect_out(39, 98 + 0 * 20, 92, 14, vga13h_Black);
			rect_out(39, 98 + 2 * 20, 92, 14, vga13h_Black);
			sleep(15);
		}
		else if (last == 0x26) // 3
		{
			rect_out(39, 98 + 2 * 20, 92, 14, vga13h_Dark_Orange);
			rect_out(39, 98 + 1 * 20, 92, 14, vga13h_Black);
			rect_out(39, 98 + 0 * 20, 92, 14, vga13h_Black);
			sleep(15);
		}
		else
		{
			asm("hlt");
		}
		
	}
	
}

user_t user;

void sel_usr() {
//visual:	
	// window
	draw_rectangle(70, 10, 160, 173, 234); // border
	draw_rectangle(75, 15, 150, 163, 240); // bg

	// icon
	fill_circle(150, 50, 30, 234);
	fill_circle(150, 38, 10, 240);
	fill_ellipse(20, 50, 150, 95, 240);

	//text
	//select user
	draw_rectangle(100, 75, 100, 20, 234);
	draw_string(110, 80, "7x12", "select user", vga13h_White);
	
	//name:
	draw_rectangle(78, 112, 134, 24, 234);
	draw_rectangle(80, 114, 130, 20, vga13h_White);
	draw_rectangle(82, 97, 40, 17, 234);
	draw_string(85, 99, "7x12", "name:", vga13h_White);

	//password:
	draw_rectangle(78, 152, 134, 24, 234);
	draw_rectangle(80, 154, 130, 20, vga13h_White);
	draw_rectangle(82, 138, 70, 16, 234);
	draw_string(85, 140, "7x12", "password:", vga13h_White);

//functionality:	
	window_t text_box = {
		20,
		130,
		3,
		234,
		vga13h_White
	};		
	unsigned char index = 0;
	NewWindow(82, 120 + (index % 2) * 38, text_box);

	unsigned char data = GetChar();
	while (data != '\n')
	{
		data = GetChar();
		if (data == '\t') {
			draw_char(82, 120 + (index % 2) * 38, "7x12", '_', text_box.background_color);
			index++;
			NewWindow(82, 120 + (index % 2) * 38, text_box);
			draw_char(82, 120 + (index % 2) * 38, "7x12", '_', text_box.text_color);
		}	
		else
		{
			asm("hlt");
		}	
	}	
	draw_rectangle(80, 114, 130, 20, vga13h_White);
	draw_rectangle(80, 154, 130, 20, vga13h_White);

	unsigned char* username = "";
	unsigned char* password = "";
	int namelen = 0;
	int passlen = 0;
	for (unsigned char i = 0; i < 2; i++)
	{
		if (index % 2 == 0) {
			username = Read_Line();	
			namelen = strlen(username) + 1;
			user.username = kmalloc(namelen);
			unsigned char* temp = user.username;
			for (int i = 0; i <= namelen; i++)
			{
				*user.username = *username;
				user.username++;
				username++;
			}
			*user.username++ = '\0';
			user.username = temp;
			SetUsrName(user.username);
		}

		if (index % 2 != 0) {
			password = Read_Line();	
			passlen = strlen(password) + 1;
			user.password = kmalloc(passlen);
			unsigned char* temp2 = user.password;
			for (int i = 0; i <= passlen; i++)
			{
				*user.password = *password;
				user.password++;
				password++;
			}
			*user.password++ = '\0';
			user.password = temp2;
			SetUsrPass(password);
		}

		index ++;
		NewWindow(82, 120 + (index % 2) * 38, text_box);
	}

	ReturnDefaultWindow();
	print_logoALT();
	set_font(6);
	set_color(vga13h_Neon_Green);
	write_string("\n\n\n\n\nWelcome ");
	write_string(user.username);
	write_string("!");
	sleep(300);
	textwindow_init();
}	
// 1 for background and text, 0 for user and path
void color_setting_gui(int switcher) {
	palate_display();

	draw_rectangle(177, 65, 46, 20, 236);
	draw_rectangle(179, 67, 42, 18, 240);
	draw_string(179, 68, "6x8", "select:", vga13h_White);

	draw_rectangle(175, 80, 120, 80, 236);
	draw_rectangle(177, 82, 116, 76, 240);

	draw_rectangle(180, 85, 110, 33, 236);
	draw_rectangle(180, 123, 110, 33, 236);
	if (switcher == 1)
	{
		draw_string(216, 95, "8x12", "text", vga13h_White);
		draw_string(193, 133, "8x12", "background", vga13h_White);
	}
	if (switcher == 0)
	{
		draw_string(216, 95, "8x12", "user", vga13h_White);
		draw_string(216, 133, "8x12", "path", vga13h_White);
	}
	

	unsigned char index = 0;
	unsigned char keycode = get_last_keycode();
	while (keycode != '\n') // enter
	{
		keycode = GetChar();

		if(keycode == '\t') { // tab
			rect_out(179, 84 + 38 * (index % 2), 111, 34, vga13h_Dark_Orange);	
			rect_out(179, 84 + 38 * ((index+1) % 2), 111, 34, 240);
			index++;	
		}
		else
		{
			asm("hlt");
		}
	}

	unsigned char offset_x = 0;
	unsigned char offset_y = 0; 
	rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, vga13h_Dark_Orange); // 16 x 16 = 256 colors

	unsigned char sel_color = read_pixel(11 + 10 * (offset_x % 16) + 4, 11 + 10 * (offset_y % 16) + 4);
	
	for (int i = 0; i < 2; i++)
	{
		KeyEvent();
		keycode = GetExtMakeCode();	
		unsigned char exit = GetMakeCode();
		while (ScancodeToAscii(exit) != '\n') // enter
		{
			KeyEvent();
			keycode = GetExtMakeCode();
			exit = GetMakeCode();
			if (keycode)
			{
				if (keycode == 0x75 && (offset_y % 16) > 0) // up arrow
				{
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, 238);
					offset_y--;
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, vga13h_Dark_Orange);
					sel_color = read_pixel(11 + 10 * (offset_x % 16) + 4, 11 + 10 * (offset_y % 16) + 4);
				}
				
				else if (keycode == 0x72 && (offset_y % 16) < 16) // down arrow
				{
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, 238);
					offset_y++;
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, vga13h_Dark_Orange);
					sel_color = read_pixel(11 + 10 * (offset_x % 16) + 4, 11 + 10 * (offset_y % 16) + 4);
				}
				
				else if (keycode == 0x74 && (offset_x % 16) < 16) // right arrow
				{
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, 238);
					offset_x++;
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, vga13h_Dark_Orange);
					sel_color = read_pixel(11 + 10 * (offset_x % 16) + 4, 11 + 10 * (offset_y % 16) + 4);
				}
				
				else if (keycode == 0x6b && (offset_x % 16) > 0) // left arrow
				{
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, 238);
					offset_x--;
					rect_out(11 + 10 * (offset_x % 16), 11 + 10 * (offset_y % 16), 9, 9, vga13h_Dark_Orange);
					sel_color = read_pixel(11 + 10 * (offset_x % 16) + 4, 11 + 10 * (offset_y % 16) + 4);
				}

				if (switcher == 1)
				{
					if (index % 2 == 1)
					{
						draw_string(216, 95, "8x12", "text", sel_color);
						draw_string(193, 133, "8x12", "background", sel_color);
					}
					else if (index % 2 == 0)
					{
						draw_rectangle(180, 123, 110, 33, sel_color);
						draw_string(193, 133, "8x12", "background", GetColor());
						draw_rectangle(180, 85, 110, 33, sel_color);
						draw_string(216, 95, "8x12", "text", GetColor());
					}
				}

				if (switcher == 0)
				{
					if (index % 2 == 1)
					{
						draw_string(216, 95, "8x12", "user", sel_color);
						draw_string(216, 133, "8x12", "path", sel_color);
					}
					else if (index % 2 == 0)
					{
						draw_rectangle(180, 123, 110, 33, sel_color);
						draw_string(216, 133, "8x12", "path", GetColor());
						draw_rectangle(180, 85, 110, 33, sel_color);
						draw_string(216, 95, "8x12", "user", GetColor());
					}
				}				
			}
			else
			{
				asm("hlt");
			}
		}	
		if (index % 2 == 1)
		{
			set_color(sel_color);
		}

		if (index % 2 == 0)
		{
			set_BGcolor(sel_color);
		}
		
		rect_out(179, 84 + 38 * (index % 2), 111, 34, vga13h_Dark_Orange);	
		rect_out(179, 84 + 38 * ((index+1) % 2), 111, 34, 240);
		index++;
	}
	clear_window();
}

void quit() {
	clear_screen();
	rect_out(168, 28, 15 * 8 + 2, 14, vga13h_White);
	draw_line(168, 24, 168, 46, vga13h_White);
	draw_line(168, 28, 168, 42, vga13h_Black);
	draw_line(168, 24, 157, 35, vga13h_White);
	draw_line(157, 35, 168, 46, vga13h_White);
	draw_string(170, 30, "8x12", "To Be Continued", vga13h_White);
	draw_line(294, 28, 294, 42, vga13h_White);
	draw_line(294, 42, 298, 42, vga13h_White);
	draw_line(294, 28, 298, 42, vga13h_White);
	draw_line(298, 28, 304, 42, vga13h_White);
	draw_line(304, 28, 304, 42, vga13h_White);
	draw_line(298, 28, 304, 28, vga13h_White);
	draw_line(306, 28, 306, 42, vga13h_White);
	draw_line(306, 28, 310, 28, vga13h_White);
	draw_line(310, 28, 306, 42, vga13h_White);


	draw_string(120, 67, "6x8", "I wouldn't leave if I were you.\n\t\t\t\t\t\t\t\t\t\tDOS is much worse.", vga13h_White);

	draw_string(130, 180, "8x12", "SEE YOU SPACE COWBOY ...", vga13h_White);

	draw_string(0, 10, "7x12", "mischief managed", vga13h_White);

	draw_string(0, 160, "6x8", "sleep mode activaited", vga13h_White);

	draw_string(30, 130, "6x8", "So long, and thanks for all the fish", vga13h_White);

	draw_string(100, 100, "12x16", "Goodbye :)", vga13h_White);

	draw_string(50, 50, "8x12", "return 0;", vga13h_White);

	sleep(300);
	outb(0xF4, 0x00);
}

void note() {
	window_t note = {
		200,
		320,
		3,
		vga13h_Dark_Blue,
		vga13h_White
	};
	NewWindow(0, 0, note);
	clear_window();
	x_line(0, 200, 300, vga13h_Red1);
	for (int y = 1; y < 14; y++)
	{
		y_line(0, 320, y * 14 - 1, vga13h_Red1);
		fill_circle(310, y * 14 - 7, 4, 234);
	}
	
	while(1) {
		if (!strcmp(Read_Line(), "Do you know anything about the chamber of secrets?")) {
			write_string("Yes\n");
			if (!strcmp(Read_Line(), "Can you tell me?"))
			{
				write_string("No...\nBut I can show you\n");
			}			
		} 
		if (!strcmp(Read_Line(), "exit")) {
			break;
		} 
	}
}

void whoami() {
	BumpX(-(strlen(user.username) + 4) * GetKeyWidth());
	write_string(user.username);
	write_string("\n");
	BumpX((strlen(user.username) + 4) * GetKeyWidth());

}

void terminal() {
	unsigned char user_color = vga13h_Blue_Sky;
	unsigned char path_color = vga13h_White; 
	unsigned char text_temp = vga13h_White;
	unsigned char bg_temp = 234;
	set_BGcolor(bg_temp);
	clear_window();
	set_color(user_color);
	write_string(user.username);
	set_color(path_color);
	write_string(":/$");
	set_color(text_temp);

	while (1)
	{
		BumpX((strlen(user.username) + 4) * GetKeyWidth());
		unsigned char* command = Read_Line(); 
		if (strcmp(command, "\n"))
		{
			if (!strcmp(command, "clear")) {
				clear_window();
			}
			else if (!strcmp(command, "color"))
			{
				color_setting_gui(1);
				text_temp = GetColor();
				bg_temp = GetBGColor();
			}
			else if (!strcmp(command, "prompt"))
			{
				color_setting_gui(0);
				user_color = GetColor();
				path_color = GetBGColor();

				set_color(text_temp);
				set_BGcolor(bg_temp);
				clear_window();
			}
			else if (!strcmp(command, "note"))
			{
				note();
				textwindow_init();		
				BumpX((strlen(user.username) + 4) * GetKeyWidth());		
				set_BGcolor(bg_temp);
				set_color(text_temp);
				clear_window();
			}
			else if (!strcmp(command, "whoami"))
			{
				whoami();
			}
			else if (!strcmp(command, "calculator")) {

			}
			else if (!strcmp(command, "quit")) {
				quit();
			}
			else if (!strcmp(command, "help"))
			{
				BumpX(-(strlen(user.username) + 4) * GetKeyWidth());
				write_string("'clear'- clears the screen\n'color'- lets you choose the text and background\ncolors\n'prompt'- change the color of the username and path \non the prompt\n");
				write_string("'note'- opens the text editor\n'whoami'- prints the current username\n'calculator'- opens the calculator\n'quit'- turns off the OS\n'help'- lists the available commands\n");
				BumpX((strlen(user.username) + 4) * GetKeyWidth());
			}
			
			else
			{
				BumpX(-(strlen(user.username) + 4) * GetKeyWidth());
				write_string("command not found: '");
				write_string(command);
				write_string("'\n");
				BumpX((strlen(user.username) + 4) * GetKeyWidth());
			}
		}
		
		BumpX(-(strlen(user.username) + 4) * GetKeyWidth());
		set_color(user_color);
		write_string(user.username);
		set_color(path_color);
		write_string(":/$");
		set_color(text_temp);
	}
}

void kernel_main(void) 
{
	//setup
	prerequisite();
	ps2_init();
	PIT_SetHz(100);
	IRQ_clear_mask(0);
	IRQ_clear_mask(1);

	set_vga_graphics_mode_2();
	clear_screen();
	textwindow_init();
	loading_screen();
	sel_usr();
	terminal();
	
}
