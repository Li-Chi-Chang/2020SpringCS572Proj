/**
 * COPYRIGHT
 * AUTHOR: Li-Chi, Chang
 * 
 * descreption:
 * This is a double linked list. 
 */

#include "linkedList.h"
#include <string.h>
#include <stdlib.h>

linkedList* initLinkedList(linkedListData data)
{
    linkedList *newHead = malloc(sizeof(linkedList));
    newHead->data = data;
    newHead->next = NULL;
    newHead->prev = NULL;
    return newHead;
}

void addNodeAtTail(linkedList* node, linkedListData data)
{
    linkedList* tail =  toListTail(node);
    linkedList* newNode  = initLinkedList(data);
    newNode->prev = tail;
    tail->next = newNode;
}

linkedList* toListTail(linkedList* node)
{
    linkedList* tail = node;
    if(tail->next != NULL)
    {
        return toListTail(tail);
    }
    return tail;
}

linkedList* toListHead(linkedList* node)
{
    linkedList* head = head;
    if(head->prev != NULL)
    {
        return toListHead(head);
    }
    return head;
}

int checkDataInList(linkedList* node, linkedListData data)
{
    linkedList* head = toListHead(node);
    while(!nodeDataEqual(head->data,data) && head != NULL)
    {
        head = head->next;
    }
    if(head == toListTail(node) && !nodeDataEqual(head->data,data))
    {
        return 0;
    }
    return 1;
}

int nodeDataEqual(linkedListData a, linkedListData b)
{
    linkedListData* pointerA = &a;
    linkedListData* pointerB = &b;
    if(!memcmp(pointerA,pointerB,sizeof(linkedListData)))
        return 1;
    return 0;
}