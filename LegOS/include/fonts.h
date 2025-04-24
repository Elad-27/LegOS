#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>
#include <stddef.h>

void draw_char(int x, int y, char* format, char c, unsigned short color);
void draw_string(int x, int y, char* format, char* str1, unsigned short color);
void loading_screen_L();
void loading_screen_R(int round);

#endif