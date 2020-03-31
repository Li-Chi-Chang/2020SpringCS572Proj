/**
 * COPYRIGHT
 * AUTHOR: Li-Chi, Chang
 * 
 * descreption:
 * This is a double linked list. 
 */

#include <stdlib.h>

typedef struct linkedList
{
    struct linkedList *prev;
    unsigned int data;
    struct linkedList *next;
}linkedList;

/**
 * descreption: init a list node
 * input: the data of this node
 * output: a node contains data and Null links
 */
linkedList* initLinkedList(unsigned int);

/**
 * descreption: add a new node at the tail of a list
 * input: the list need to be added a node and the data of this node
 * output: void
 */
void addNodeAtTail(linkedList*, unsigned int);

/**
 * descreption: go to the end of this list
 * input: the list need to be run
 * output: the end node of this list
 */
linkedList* toListTail(linkedList*);

/**
 * descreption: go to the head of this list
 * input: the list need to be run
 * output: the head node of this list
 */
linkedList* toListHead(linkedList*);

/**
 * descreption: check a number is contained in this list or not
 * input: a node in a list, and the searched data
 * output: 1 found / 0 failed
 */
int checkDataInList(linkedList*, unsigned int);
