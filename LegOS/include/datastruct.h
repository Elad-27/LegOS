#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))
#define checkTop(sp) (*(sp - 1))

void Empty_stack(int stack[], int *sp);

#endif