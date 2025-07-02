#ifndef MATHLIB_H
#define MATHLIB_H

#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include "pit.h"

#define math_Pi 3.141592653589793 //15 digits after the decimal, should be good enough.
#define math_e	2.718281828459045 //might as well if I'm at it.

#define predict_false(exp)    __builtin_expect((exp), 0)

#define FORCE_EVAL(x) do {                        \
	if (sizeof(x) == sizeof(float)) {             \
		volatile float __x;                       \
		__x = (x);                                \
	} else if (sizeof(x) == sizeof(double)) {     \
		volatile double __x;                      \
		__x = (x);                                \
	} else {                                      \
		volatile long double __x;                 \
		__x = (x);                                \
	}                                             \
} while(0)

 
int sqrt(int square);
int abs(int n);
double powerof(double base, int power);
double factorial(int num);
unsigned char random();
double __cos(double x, double y);
double __sin(double x, double y, int iy);
int __rem_pio2(double x, double *y);
double Math_Sin(double x);
double Math_Cos(double x);

#endif