#include "singleStackInt.h"

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
        return stackerr(NOINITSTACKERR);
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
        return stackerr(NOINITSTACKERR);
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
        return stackerr(STACKEMPTY);
    }
}

int getStackLen()
{
    return stackbase.data;
}

/**
 * 
 */
int stackerr(int errcode)
{
    if(errcode == 0)
    {
        return 0;
    }

    printf("Error here.\nerr code: ");
    switch (errcode)
    {
    case NOINITSTACKERR:
        printf("%d NO INIT STACK ERR!\n",errcode);
        break;
    case STACKEMPTY:
        printf("%d STACK EMPTY!\n",errcode);
        break;
    case ALLOCATIONERR:
        printf("%d ALLOCATION ERR!\n",errcode);
        break;
    default:
        printf("unknown %d\n",errcode);
    }
    exit(1);
}