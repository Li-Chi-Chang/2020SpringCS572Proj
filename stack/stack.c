#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int stackerr(int);

int initStack = 0;

// init for this stack
stackBase* initStackInt()
{
    initStack = 1;
    stackBase* base = malloc(sizeof(stackBase));
    base->count = 0;
    base->next = NULL;
    return base;
}

/**
 * return 0: success
 * return 1: error on stack
 * */
int push(stackBase* base, stackData data)
{
    if(!initStack)
    {
        return stackerr(NOINITSTACKERR);
    }

    stackNode *newNode = (stackNode*) malloc(sizeof(stackNode));
    if(newNode == NULL)
    {
        return stackerr(ALLOCATIONERR);
    }

    if(base->next == NULL)
    {
        base->next = newNode;
    }
    else
    {
        stackNode* pointerNode = base->next;
        while(pointerNode->next != NULL)
        {
            pointerNode = pointerNode->next;
        }
        pointerNode->next = newNode;
        newNode->prev = pointerNode;
    }
    newNode->data = data;
    newNode->next = NULL;
    base->count++;
    return 0;
}

stackData pop(stackBase* base)
{
    stackData data;

    if(!initStack)
    {
        stackerr(NOINITSTACKERR);
        return data;
    }

    if(base->count != 0)
    {
        base->count--;
        stackNode* pointerNode = base->next;
        while(pointerNode->next != NULL)
        {
            pointerNode = pointerNode->next;
        }
        pointerNode->prev->next = NULL;
        data = pointerNode->data;
        base->count--;
        free(pointerNode);
        return data;
    }
    else
    {
        stackerr(STACKEMPTY);
        return data;
    }
}

int getStackLen(stackBase* base)
{
    return base->count;
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