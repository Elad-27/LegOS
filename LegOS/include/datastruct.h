#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include "ps2_keyboard.h"

#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))
#define checkTop(sp) (*(sp - 1))

#define Empty_stack(sp, stack) (sp = stack)

#endif