/**
 * COPYRIGHT
 * AUTHOR: Li-Chi, Chang
 * 
 * descreption:
 * This is a double linked list. 
 */

#include <stdlib.h>

/**
 * can change it if needed
 */
typedef struct treeData
{
    unsigned int data;
}binaryTreeData;

typedef struct tree
{
    struct tree *parent;
    struct treeData data;
    struct tree *leftChild;
    struct tree *rightChild;
}binaryTree;


/**
 * descreption: init a tree node
 * input: the data of this node
 * output: a node contains data and Null links
 */
binaryTree* initTreeNode(binaryTreeData);

/**
 * descreption: go to the head of this tree
 * input: the tree need to be run
 * output: the root node of this list
 */
binaryTree* toTreeRoot(binaryTree*);