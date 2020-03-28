#include "stackInt.h"

int initStack = 0;

// private struct for the stack
typedef struct stackNode
{
    unsigned int data;
    struct stackNode *next;
}stackNode;

stackNode stackbase = {0,NULL};
stackNode *stackTop;

// init for this stack
void initStackInt()
{
    initStack = 1;
    stackTop = &stackbase;
}

/**
 * return 0: success
 * return 1: error on stack
 * */
int push(unsigned int data)
{
    if(!initStack)
    {
        return err(NOINITERR);
    }
    stackNode *newNode = (stackNode*) malloc(sizeof(stackNode));
    if(newNode == NULL)
    {
        return err(ALLOCATIONERR);
    }
    newNode->data = data;
    newNode->next = stackTop;
    stackTop = newNode;
    stackbase.data++;
    return 0;
}

unsigned int pop()
{
    if(!initStack)
    {
        return err(NOINITERR);
    }
    if(stackbase.data != 0)
    {
        stackbase.data--;
        unsigned int data = stackTop->data;
        stackTop = stackTop->next;
        return data;
    }
    else
    {
        return err(STACKEMPTY);
    }
}

int getStackLen()
{
    return stackbase.data;
}