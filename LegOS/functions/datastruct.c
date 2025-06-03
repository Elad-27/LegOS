#include "../include/datastruct.h"
#include <stdbool.h>

void Empty_stack(int stack[], int *sp) {
    for (int i = 0; i < sp - stack; i++)
    {
        pop(sp);
    }
}