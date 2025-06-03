#include "../include/shapes.h"
#include "../include/mathlib.h"
#include "../include/VGA_driver.h"

#include <stdint.h>
#include <stddef.h>


void draw_rectangle(int x, int y, int width, int height, unsigned short color) {
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i <= 320 && j <= 200) {
				draw_pixel(x + i, y + j, color);
			}
		}
		
	}
	
}

void rect_out(int x, int y, int width, int height, unsigned short color) {
	for (int i = 0; i <= width; i ++) {
		draw_pixel(x + i, y, color);
		draw_pixel(x + i, y + height, color);
	}

	for (int i = 0; i < height; i ++) {
		draw_pixel(x, y + i, color);
		draw_pixel(x + width, y + i, color);
	}
}

void draw_circle(int x, int y, int r, unsigned short color) { // (x - a)^2 + (y - b)^2 = r^2, where the point (a,b) is the center of the circle with a radius - r
	for (int i = x - r; i <= x + r; i++)
	{
		for (int j = y - r; j <= y + r; j++)
		{
			if (((sqrt(((i - x) * (i - x) + (j - y) * (j - y))) == r) && i <= 320 && j <= 200)) { 
				draw_pixel(i, j, color);													
			}
		}
	}
	
}

unsigned char* get_circle_bitmap(int x, int y, int r) { // converts to an 8bit bitmap 
	unsigned char* bitmap;
	char data = 0;
	int size = 0;
	int count = 0;
	for (int i = x - r; i <= x + r; i++)
	{
		for (int j = y - r; j <= y + r; j++)
		{
			if (((sqrt(((i - x) * (i - x) + (j - y) * (j - y))) == r) && i <= 320 && j <= 200)) { 
				data += 1;
			}
			data << 1;
			size++;
			if (size == 7) {
				bitmap[count] = data;
				size = 0;
				data = 0;
				count++;
			}
		}
	}

	return bitmap;
}

void print_circle_bitmap(int x, int y, int r, unsigned short color, char* bitmap) {
	char data;
	for (int i = x - r; i <= x + r; i++)
	{
		for (int j = y - r; j <= y + r; j++)
		{
			data = bitmap[i];
			for (int k = 0; k < 8; k++)
			{
				if ( (data >> 7 - k) & 1 )
				{
					draw_pixel(i, j, color);
				}
			}
		}
	}
}

void fill_circle(int x, int y, int r, unsigned short color) {
	for (int i = x - r; i <= x + r; i++)
	{
		for (int j = y - r; j <= y + r; j++)
		{
			if ((sqrt(((i - x) * (i - x) + (j - y) * (j - y))) <= r) && i <= 320 && j <= 200) {
				draw_pixel(i, j, color);
			}
		}
		
	}
	
}
/**
 * * note: fill_circle() and draw_circle() have been observed to overflow into the other side of the screen,
 * * e.g: circle at the top right corner gets cut and the rest of it appears on the top left corner.
 * * I think this also happens with other functions like the lines. Need to get to the buttom of this.   
*/


void draw_line(double m, int n, unsigned short color) { // y = m*x + n
	int x_end = 200 / m - n;
	x_end = 320 ^ ((x_end ^ 320) & -(x_end < 320));
	int y = 0;
	for (int x = 0; x <= x_end; x++)
	{
		for (y = n; y <= 200; y++)
		{
			if (y == m * x + n)
			{
				draw_pixel(x, y, color);
			}
		}
	}
}// Becuase of the nature of VGA 13h mode, translation in the x axis remains the same as in the cartesian plane (positive is to the right),
 // but going "forward" in the y axis actually goes down, (0,0) is in the top left and (320,200) is in the buttom right.
 // Therefor, when plotting a graph such as y = mx + n , it will actually look upside down.
 // (unless I actively deside to implament a more human-readable x-y plane where the (0,0) will be around the middle and going in the normal directions)
 // (but this is sort of moot concidering it will just make extra abstractions in places where we need as few as possible, best to save this to stuff like a graphing app in userspace)

void x_line(int x, unsigned short color) {
	for (int y = 0; y < 200; y++)
	{
		draw_pixel(x, y, color);
	}
	
}

void y_line(int y, unsigned short color) {
	for (int x = 0; x < 320; x++)
	{
		draw_pixel(x, y, color);
	}
	
}

void draw_ellipse(int major_axis, int minor_axis, int x, int y, unsigned short color) {
	for (int i = x - major_axis; i <= x + major_axis; i++) {
		for (int j = y - minor_axis; j <= y + minor_axis; j++) {
			int value = ((i - x) * (i - x)) * (minor_axis * minor_axis) + ((j - y) * (j - y)) * (major_axis * major_axis);
			if ((value >= major_axis * major_axis * minor_axis * minor_axis - (major_axis * minor_axis * 100)) && (value <= major_axis * major_axis * minor_axis * minor_axis)) {
				draw_pixel(i , j, color);
			}
		}
	}
}

void fill_ellipse(int major_axis, int minor_axis, int x, int y, unsigned short color) {
	for (int i = x - major_axis; i <= x + major_axis; i++) {
		for (int j = y - minor_axis; j <= y + minor_axis; j++) {
			if (((i - x) * (i - x)) * (minor_axis * minor_axis) + ((j - y) * (j - y)) * (major_axis * major_axis)  <= major_axis * major_axis * minor_axis * minor_axis) {
				draw_pixel(i , j, color);
			}
		}
	}
}
