#ifndef SHAPES_H
#define SHAPES_H

#include <stdint.h>
#include <stddef.h>

void draw_rectangle(int x, int y, int width, int height, unsigned short color);
void draw_circle(int x, int y, int r, unsigned short color);
unsigned char* get_circle_bitmap(int x, int y, int r);
void print_circle_bitmap(int x, int y, int r, unsigned short color, char* bitmap);
void fill_circle(int x, int y, int r, unsigned short color);
void draw_line(double m, int n, unsigned short color);
void x_line(int x, unsigned short color);
void y_line(int x, unsigned short color);
void draw_ellipse(int major_axis, int minor_axis, int x, int y, unsigned short color);
void fill_ellipse(int major_axis, int minor_axis, int x, int y, unsigned short color);

#endif