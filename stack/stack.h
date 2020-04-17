/**
 * COPYRIGHT
 * AUTHOR: Li-Chi, Chang
 * 
 * descreption:
 * This is a single int stack
 * simply use push, pop
 * but befor using, please init it, or it will has error
*/

typedef struct stackData
{
    unsigned int data;
}stackData;

// struct for the stack
typedef struct stackNode
{
    struct stackNode *prev;
    stackData data;
    struct stackNode *next;
}stackNode;

typedef struct
{
    unsigned int count;
    struct stackNode *next;
}stackBase;

/**
 * descreption: init this stack
 * input:
 * output: a stack base
 */
stackBase* initStackInt();

/**
 * descreption: push data into the stack
 * input: data in unsigned int format
 * output: 0:success, 1 err
 */
int push(stackBase*, stackData);

/**
 * descreption: pop the top layer of this stack
 * input:
 * output: data in unsigned int
 */
stackData pop(stackBase*);

/**
 * descreption: get the length of this stack
 * input:
 * output: the length of this stack
 */
int getStackLen(stackBase*);

/**
 * descreption: error code
 */
#define STACKEMPTY 0xFFFFFFFF
#define ALLOCATIONERR 0x04
#define NOINITSTACKERR 0x01