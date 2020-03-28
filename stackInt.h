/**
 * This is a single int stack
 * simply use push, pop
 * but befor using, please init it, or it will has error
*/

#include <stdlib.h>
#include "err.h"

void initStackInt();
int push(unsigned int);
unsigned int pop();
int getStackLen();