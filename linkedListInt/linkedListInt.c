/**
 * COPYRIGHT
 * AUTHOR: Li-Chi, Chang
 * 
 * descreption:
 * This is a double linked list. 
 */

#include "linkedListInt.h"

linkedList* initLinkedList(unsigned int data)
{
    linkedList *newHead = malloc(sizeof(linkedList));
    newHead->data = data;
    newHead->next = NULL;
    newHead->prev = NULL;
    return newHead;
}

void addNodeAtTail(linkedList* node, unsigned int data)
{
    linkedList* tail =  toListTail(node);
    linkedList* newNode  = initLinkedList(data);
    newNode->prev = tail;
    tail->next = newNode;
    return tail;
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

int checkDataInList(linkedList* node, unsigned int data)
{
    linkedList* head = toListHead(node);
    while(head->data != data)
    {
        head = head->next;
    }
    if(head == toListTail(node) && head->data != data)
    {
        return 0;
    }
    return 1;
}
