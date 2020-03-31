/**
 * COPYRIGHT
 * AUTHOR: Li-Chi, Chang
 * 
 * descreption:
 * This is a single int stack
 * simply use push, pop
 * but befor using, please init it, or it will has error
*/

#include <stdlib.h>

/**
 * descreption: init this stack
 * input:
 * output:
 */
void initStackInt();

/**
 * descreption: push data into the stack
 * input: data in unsigned int format
 * output:
 */
int push(unsigned int);

/**
 * descreption: pop the top layer of this stack
 * input:
 * output: data in unsigned int
 */
unsigned int pop();

/**
 * descreption: get the length of this stack
 * input:
 * output: the length of this stack
 */
int getStackLen();

/**
 * descreption: error code
 */
#define STACKEMPTY 0xFFFFFFFF
#define ALLOCATIONERR 0x04
#define NOINITSTACKERR 0x01