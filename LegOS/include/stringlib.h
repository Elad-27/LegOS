#ifndef STRINGLIB_H
#define STRINGLIB_H

#include <stdint.h>
#include <stddef.h>
#include "fonts.h"

int strcmp(const char* s1, const char* s2);
int strlen(const char* str);
char* strcomb(char* s1, char* s2);
char* ToString(int value, char* result, int base);
char* ToStringFloat(double value, char* result, int decimalplaces);
unsigned char* strcat_c(char *str, char c);

#endif