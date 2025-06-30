#include "../include/stringlib.h"
#include "../include/fonts.h"
#include "../include/mathlib.h"
#include <stdint.h>
#include <stddef.h>

//returns the length of a string
int strlen(const char* str) {
    int len = 0;
    while(*str){
        len++;
        *str++;
    }
    return len;
}

//compares if two strings are the same
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

//combines two strings: "[s1][s2]"
char* strcomb(char* s1, char* s2) {
    while (*s1) {
        s1++;
    }
    while (*s2) {
        *s1 = *s2;
        s1++;
        s2++;
    }
}

//converts an interger to a string
char* ToString(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
  
    // Reverse the string
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

// converts a floating point / double number to a string
char* ToStringFloat(double value, char* result, int decimalplaces) {
    decimalplaces = decimalplaces % 10;
    char* ptr = result;
    char* ptr1 = result;
    int temp = powerof(10, decimalplaces) * value;

    unsigned char negative = 0;
    if (temp * 10 < 0)
    {
        negative = 1;
    }
    
    for (int i = 0; i < decimalplaces; i++)
    {
        *ptr++ = '0' + abs(temp) % 10;
        temp /= 10;
    }
    *ptr++ = '.';
    while (abs(temp) >= 10)
    {
        *ptr++ = '0' + abs(temp) % 10;
        temp /= 10;
    }
    *ptr++ = '0' + abs(temp);
    if (negative) *ptr++ = '-';
    *ptr-- = '\0';
    
    while(ptr1 < ptr) {
        temp = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = temp;
    }

    return result;
}

//add a char to a string
unsigned char* strcat_c(char *str, char c) {
    for (;*str;str++);
    *str++ = c; 
    *str++ = '\0';

    return str;
}