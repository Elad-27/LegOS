#ifndef SHAPES_H
#define SHAPES_H

#include <stdint.h>
#include <stddef.h>

typedef struct point
{
    int x;
    int y;
}point_t;


void draw_rectangle(int x, int y, int width, int height, unsigned short color);
void rect_out(int x, int y, int width, int height, unsigned short color);
void draw_circle(int x, int y, int r, unsigned short color);
unsigned char* get_circle_bitmap(int x, int y, int r);
void print_circle_bitmap(int x, int y, int r, unsigned short color, char* bitmap);
void fill_circle(int x, int y, int r, unsigned short color);
void draw_line(int x0, int y0, int x1, int y1, unsigned short color);
void x_line(unsigned char start, unsigned char height, unsigned short x, unsigned short color);
void y_line(unsigned short start, unsigned short width, unsigned char y, unsigned short color);
void draw_ellipse(int major_axis, int minor_axis, int x, int y, unsigned short color);
void fill_ellipse(int major_axis, int minor_axis, int x, int y, unsigned short color);
void draw_polygon(point_t points[], unsigned char color);
void fill_polygon(point_t points[], unsigned char color);

#endif