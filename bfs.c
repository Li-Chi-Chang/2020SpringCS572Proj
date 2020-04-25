#include "map.h"
#include "stack_C/stack.h"
#include "linkedList_C/linkedList.h"
#include "binaryTree_C/binaryTree.h"

//basic actions
void turn(char);//turn a direction
void go();//go forward one step
mapNode getRealDirection(mapNode, int);

//advenced actions
int goToNextBranch();//go stregth to next fork node
void faceTo(int);//face to one of NEWS
binaryTree* findNodeWithDfsWithLocation(binaryTree*, binaryTreeData);//input pointer, data, return pointer
int getBottom(int);
void goToDest(binaryTree*, binaryTree*);//from this node to other node in tree, second input is the path
int goToDestAlgo(binaryTree*, binaryTreeData, binaryTree*);//the third input is an output

//algo
void dfs();

//others
void init();
binaryTreeData initTreeData();
/**
 * descreption: print this subtree
 * input: root,recurrsiveOrNot(1:yes, 0:no)
 * output: not def now
 */
int printTreeText(binaryTree*,int);
void printStackText(stackBase*);
stackData popWithText(stackBase*);
int pushWithText(stackBase*, stackData);
void printCurrentNode(mapNode, int);



char mousehead = NORTH;
int location[2] = {0,0};// 1:x 0:y
binaryTree* tree;
stackBase* stack;

//for debug
char string[1024];

#define HEIGHT 0
#define WIDTH 1

#define UNDEFINE 99
#define ENTRYWAY 100
#define HITWALL 98
#define TRAVELED 3
#define CROSSED 101

int main(void)
{
    // must init first
    init();
    // do the bfs algo
    mapNode current = getRealDirection(getNodeInfo(),mousehead);
    stackData data;
    data.locationX = location[WIDTH];
    data.locationY = location[HEIGHT];

    printCurrentNode(getNodeInfo(),mousehead);
    switch(current.east)
    {
    case GRID:
        data.travelDir = EAST;
        pushWithText(stack,data);
        break;
    }
    switch(current.west)
    {
    case GRID:
        data.travelDir = WEST;
        pushWithText(stack,data);
        break;
    }
    switch(current.north)
    {
    case GRID:
        data.travelDir = NORTH;
        pushWithText(stack,data);
        break;
    }
    switch(current.south)
    {
    case GRID:
        data.travelDir = SOUTH;
        pushWithText(stack,data);
        break;
    }
    tree->data.location[WIDTH] = location[WIDTH];
    tree->data.location[HEIGHT] = location[HEIGHT];
    tree->data.parentDir = SOUTH;
    dfs(location[WIDTH],location[HEIGHT],NORTH);
    return 0;
}

void init()
{
    // init
    readMaze();
    initErrLog();
    binaryTreeData data = initTreeData(location[WIDTH],location[HEIGHT]);
    tree = initTreeNode(data);
    stack = initStackBase();
}


void dfs(int parentX, int parentY, int exitDir)
{
    //tree pointer needs to be place at the current node
    logFile("DFS function");
    if(!getStackLen(stack))
    {
        logFile("Finish map search");
        exit(1);
    }
    stackData stackinfo =  popWithText(stack);
    printCurrentNode(getNodeInfo(),mousehead);
    printTreeText(toTreeRoot(tree),1);
    //check the path is searched or not
    binaryTree* pointer = findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(location[WIDTH],location[HEIGHT]));
    //if exist, check the path is searched
    if((pointer->data.crossDir == stackinfo.travelDir || pointer->data.leftChildDir == stackinfo.travelDir || pointer->data.rightChildDir == stackinfo.travelDir || pointer->data.parentDir == stackinfo.travelDir) && pointer->data.location[WIDTH] == stackinfo.locationX && pointer->data.location[HEIGHT] == stackinfo.locationY)
    {
        //do nothing
        logFile("This path is searched");
    }
    //if the path is not searched
    else
    {
        //make sure we are in the node
        if(tree->data.location[WIDTH] == stackinfo.locationX && tree->data.location[HEIGHT] == stackinfo.locationY)
        {
            faceTo(stackinfo.travelDir);
            go();
            goToNextBranch();
            //check if the node is not in the tree
            if(findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(location[WIDTH],location[HEIGHT])) == NULL)
            {
                if(tree->leftChild != NULL && tree->rightChild != NULL)
                {
                    logFile("out of children number!");
                    exit(1);
                }
                //init a tree node
                binaryTree* newNode = initTreeNode(initTreeData(location[WIDTH],location[HEIGHT]));
                newNode->data.parentDir = getBottom(mousehead);
                newNode->data.parentX = tree->data.location[WIDTH];
                newNode->data.parentY = tree->data.location[HEIGHT];
                newNode->parent = tree;
                if(tree->leftChild == NULL)
                {
                    tree->leftChild = newNode;
                    tree->data.leftChildDir = stackinfo.travelDir;
                    tree->data.leftChildX = newNode->data.location[WIDTH];
                    tree->data.leftChildY = newNode->data.location[HEIGHT];
                }
                else
                {
                    tree->rightChild = newNode;
                    tree->data.rightChildDir = stackinfo.travelDir;
                    tree->data.rightChildX = newNode->data.location[WIDTH];
                }
                //set the pointer to current node
                tree = newNode;
                //push the unsearched path to stack
                mapNode current = getRealDirection(getNodeInfo(),mousehead);

                stackData data;
                data.locationX = location[WIDTH];
                data.locationY = location[HEIGHT];
                if(current.south == GRID && tree->data.parentDir != SOUTH)
                {
                    data.travelDir = SOUTH;
                    pushWithText(stack,data);
                }
                if(current.west == GRID && tree->data.parentDir != WEST)
                {
                    data.travelDir = WEST;
                    pushWithText(stack,data);
                }
                if(current.east == GRID && tree->data.parentDir != EAST)
                {
                    data.travelDir = EAST;
                    pushWithText(stack,data);
                }
                if(current.north == GRID && tree->data.parentDir != NORTH)
                {
                    data.travelDir = NORTH;
                    pushWithText(stack,data);
                }
            }
            //if the node is in the tree
            else
            {
                binaryTree* crossPointer = findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(location[WIDTH],location[HEIGHT]));
                //if the cross is not empty
                if(tree->cross != NULL)
                {
                    logFile("out of corss number!");
                    exit(1);
                }
                //if the cross is empty
                else
                {
                    //data at tree side
                    tree->cross = crossPointer;
                    tree->data.crossDir = stackinfo.travelDir;
                    tree->data.crossX = crossPointer->data.location[WIDTH];
                    tree->data.crossY = crossPointer->data.location[HEIGHT];
                    //data at cross side
                    crossPointer->cross = tree;
                    crossPointer->data.crossDir = getBottom(mousehead);
                    crossPointer->data.crossX = tree->data.location[WIDTH];
                    crossPointer->data.crossY = tree->data.location[HEIGHT];
                    //set the tree location to crossside
                    tree = crossPointer;
                }   
            }
        }
        // if we are out of node
        else
        {
            //go to the node
            logFile("out of node");
            binaryTree* path = initTreeNode(initTreeData(UNDEFINE,UNDEFINE));
            goToDestAlgo(tree,initTreeData(stackinfo.locationX,stackinfo.locationY),path);
            goToDest(tree,path);
            tree = findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(stackinfo.locationX,stackinfo.locationY));
            //pushback the stackinfo because we didnt do anything but change the location
            pushWithText(stack,stackinfo);
        }
    }
    dfs(location[WIDTH],location[HEIGHT],mousehead);
}

void printCurrentNode(mapNode current, int head)
{
    mapNode realnode = getRealDirection(current,head);
    sprintf(LogFileString,"current location:    (%2d,%2d)",location[WIDTH],location[HEIGHT]);
    logFile(LogFileString);
    sprintf(LogFileString,"current NESW:        (%2d,%2d,%2d,%2d)",realnode.north,realnode.east,realnode.south,realnode.west);
    logFile(LogFileString);
    sprintf(LogFileString,"mouse NESW:          (%2d,%2d,%2d,%2d) head: %d\n",current.north,current.east,current.south,current.west,mousehead);
    logFile(LogFileString);
}

/**
 * return 0 go to a normal node
 * return 1 hit the entry
 * algo: go until meet branch
 * If meet a brach, go the right side
 * using the bfs algo
 * */
int goToNextBranch()
{
    mapNode current = getNodeInfo();
    int way = 0;
    int entry = 0;
    if(current.north == GRID || ( !ISTRAVELALL&&current.north == ENTRY))
    {
        way++;
        if(current.north == ENTRY)
            entry++;
    }
    if(current.west == GRID|| ( !ISTRAVELALL&&current.west == ENTRY))
    {
        way++;
        if(current.west == ENTRY)
            entry++;
    }
    if(current.east == GRID|| ( !ISTRAVELALL&&current.east == ENTRY))
    {
        way++;
        if(current.east == ENTRY)
            entry++;
    }
    
    switch (way)
    {
    case 0:
        //hit a wall
        return HITWALL;
        break;
    case 1:
        if(current.north == GRID)
            ;
        else if(current.east == GRID)
        {
            turn(RIGHT);
        }
        else if(current.west == GRID)
        {
            turn(LEFT);
        }
        go();
        if(location[WIDTH] == 19 && location[HEIGHT]==0)
        {
            sprintf(LogFileString,"location:gotoNB x:%d, y:%d way:%d, entry:%d", location[WIDTH],location[HEIGHT],way,entry);
            logFile(LogFileString);
            printCurrentNode(getNodeInfo(),mousehead);
        }
        
        return goToNextBranch();
    case 2:
        if(entry)
            return 1;
        return 0;
    }
    //never reach here
    return err(COMMANDNOTFOUND);
}

void turn(char dir)
{
    int head = mousehead;
    switch (dir)
    {
    case RIGHT:
        head++;
        if(head > WEST)
        {
            head = NORTH;
        }
        break;
    case LEFT:
        head--;
        if(head < NORTH)
        {
            head = WEST;
        }
        break;
    }
    mousehead = head;
    rotate(dir);
}

void go()
{
    switch (mousehead)
    {
    case NORTH:
    if(goNext())
        location[HEIGHT]++;
        break;
    case SOUTH:
    if(goNext())
        location[HEIGHT]--;
        break;
    case EAST:
    if(goNext())
        location[WIDTH]++;
        break;
    case WEST:
    if(goNext())
        location[WIDTH]--;
        break;
    }    
}

void faceTo(int dir)
{
    int rL = ((mousehead - dir)+4) % 4;
    
    switch (rL)
    {
    case 1:
        turn(LEFT);
        break;
    case 3:
        turn(RIGHT);
        break;
    case 2:
        turn(LEFT);
        turn(LEFT);
        break;
    default:
        break;
    }
}

mapNode getRealDirection(mapNode fromMouseView, int head)
{
    mapNode RealDir;
    switch (head)
    {
    case NORTH:
        RealDir.north = fromMouseView.north;
        RealDir.south = fromMouseView.south;
        RealDir.east = fromMouseView.east;
        RealDir.west = fromMouseView.west;
        break;
    case SOUTH:
        RealDir.north = fromMouseView.south;
        RealDir.south = fromMouseView.north;
        RealDir.east = fromMouseView.west;
        RealDir.west = fromMouseView.east;
        break;
    case WEST:
        RealDir.north = fromMouseView.east;
        RealDir.south = fromMouseView.west;
        RealDir.east = fromMouseView.south;
        RealDir.west = fromMouseView.north;
        break;
    case EAST:
        RealDir.north = fromMouseView.west;
        RealDir.south = fromMouseView.east;
        RealDir.east = fromMouseView.north;
        RealDir.west = fromMouseView.south;
        break;
    }
    return RealDir;
}

binaryTreeData initTreeData(int x, int y)
{
    binaryTreeData data;
    data.parentX = UNDEFINE;
    data.parentY = UNDEFINE;
    data.parentDir = UNDEFINE;
    data.leftChildX = UNDEFINE;
    data.leftChildY = UNDEFINE;
    data.leftChildDir = UNDEFINE;
    data.rightChildX = UNDEFINE;
    data.rightChildY = UNDEFINE;
    data.rightChildDir = UNDEFINE;
    data.crossX = UNDEFINE;
    data.crossY = UNDEFINE;
    data.crossDir = UNDEFINE;
    data.location[WIDTH] = x;
    data.location[HEIGHT] = y;
    return data;
}

binaryTree* findNodeWithDfsWithLocation(binaryTree* root, binaryTreeData data)
{
    binaryTree* returnInt;
    binaryTree* nextLevel = root;
    if(root->data.location[0] == data.location[0] && root->data.location[1] == data.location[1])
    {
        return root;
    }
    else if(root->leftChild == NULL && root->rightChild == NULL)
    {
        return NULL;
    }
    if(root->leftChild != NULL)
    {
        nextLevel = root->leftChild;
        returnInt = findNodeWithDfsWithLocation(nextLevel, data);
        if(returnInt != NULL)
        {
            return returnInt;
        }
    }
    if(root->rightChild != NULL)
    {
        nextLevel = root->rightChild;
        returnInt =  findNodeWithDfsWithLocation(nextLevel, data);
        if(returnInt != NULL)
        {
            return returnInt;
        }
    }
    return NULL;
}

int getBottom(int head)
{
    switch (head)
    {
    case NORTH:
        return SOUTH;
    case SOUTH:
        return NORTH;
    case EAST:
        return WEST;
    case WEST:
        return EAST;
    }
    return err(COMMANDNOTFOUND);
}

int printTreeText(binaryTree* root, int recurrsiveOrNot)
{
    sprintf(LogFileString,"this node:      (%2d,%2d)", root->data.location[WIDTH],root->data.location[HEIGHT]);
    logFile(LogFileString);

    if(root->parent==NULL)
    {
        sprintf(LogFileString,"\tparent:       NULL");
        logFile(LogFileString);
    }
    else
    {
        sprintf(LogFileString,"\tparent:       (%2d,%2d)   dir:%d",root->data.parentX,root->data.parentY, root->data.parentDir);
        logFile(LogFileString);
    }
    if(root->leftChild==NULL)
    {
        sprintf(LogFileString,"\tleftChild:    NULL");
        logFile(LogFileString);
    }
    else
    {
        sprintf(LogFileString,"\tleftChild:    (%2d,%2d)   dir:%d",root->data.leftChildX,root->data.leftChildY,root->data.leftChildDir);
        logFile(LogFileString);
    }
    if(root->rightChild==NULL)
    {
        sprintf(LogFileString,"\trightChild:   NULL");
        logFile(LogFileString);
    }
    else
    {
        sprintf(LogFileString,"\trightChild:   (%2d,%2d)   dir:%d",root->data.rightChildX,root->data.rightChildY,root->data.rightChildDir);
        logFile(LogFileString);
    }
    if(root->cross!=NULL)
    {
        sprintf(LogFileString,"\tcross:        (%2d,%2d)   dir:%d",root->data.crossX,root->data.crossY,root->data.crossDir);
        logFile(LogFileString);
    }
    if(root->rightChild==NULL && root->leftChild==NULL)
    {
        return 0;
    }
    if(root->leftChild!=NULL && recurrsiveOrNot)
    {
        printTreeText(root->leftChild, 1);
    }
    if(root->rightChild!=NULL && recurrsiveOrNot)
    {
        printTreeText(root->rightChild, 1);
    }
    return 0;
}

void printStackText(stackBase* base)
{
    sprintf(LogFileString, "Print Stack(%d):",getStackLen(base));
    logFile(LogFileString);
    logFile("base");
    stackNode* pointer = base->next;
    int i;
    for(i = getStackLen(base); i>0; i--)
    {
        sprintf(LogFileString, "(%d,%d),dir:%d",pointer->data.locationX,pointer->data.locationY,pointer->data.travelDir);
        logFile(LogFileString);
        pointer = pointer->next;
    }
    logFile("top\n");
}

stackData popWithText(stackBase* base)
{
    stackData data = pop(base);
    sprintf(LogFileString,"pop     (%2d,%2d),dir:%d\n", data.locationX,data.locationY,data.travelDir);
    logFile(LogFileString);
    return data;
}

int pushWithText(stackBase* base, stackData data)
{
    sprintf(LogFileString,"push    (%2d,%2d),dir:%d\n", data.locationX,data.locationY,data.travelDir);
    logFile(LogFileString);
    return push(base, data);
}

void goToDest(binaryTree* treeNode, binaryTree* path)
{
    int parentX = UNDEFINE, parentY = UNDEFINE;
    do{
        sprintf(LogFileString,"location: x:%d, y:%d", location[WIDTH],location[HEIGHT]);
        logFile(LogFileString);
        if(path->leftChild!=NULL && path->data.leftChildX!=parentX && path->data.leftChildY!=parentY)
        {logFile("LogFileString1");
            //set mouse
            faceTo(treeNode->data.leftChildDir);
            sprintf(LogFileString,"location:2 x:%d, y:%d", location[WIDTH],location[HEIGHT]);
            logFile(LogFileString);
            go();
            sprintf(LogFileString,"location:2 x:%d, y:%d", location[WIDTH],location[HEIGHT]);
            logFile(LogFileString);
            goToNextBranch();
            //set tree pointer
            parentX = path->data.location[WIDTH];
            parentY = path->data.location[HEIGHT];
            path = path->leftChild;
            treeNode = treeNode->leftChild;
            sprintf(LogFileString,"location:2 x:%d, y:%d", location[WIDTH],location[HEIGHT]);
            logFile(LogFileString);
        }
        else if(path->rightChild!=NULL && path->data.rightChildX!=parentX && path->data.rightChildY!=parentY)
        {logFile("LogFileString2");
            //set mouse
            faceTo(treeNode->data.rightChildDir);
            go();
            goToNextBranch();
            //set tree pointer
            parentX = path->data.location[WIDTH];
            parentY = path->data.location[HEIGHT];
            path = path->rightChild;
            treeNode = treeNode->rightChild;
        }
        else if(path->parent!=NULL && path->data.parentX!=parentX && path->data.parentY!=parentY)
        {logFile("LogFileString3");
            //set mouse
            faceTo(treeNode->data.parentDir);
            go();
            goToNextBranch();
            //set tree pointer
            parentX = path->data.location[WIDTH];
            parentY = path->data.location[HEIGHT];
            path = path->parent;
            treeNode = treeNode->parent;
        }
        else
        {
            break;
        }
        logFile("in while");
        printTreeText(treeNode,0);
    }while(1);
}

/**
 * input the current node and dest Data
 * pls use new tree to get the log, or it will break the current tree
 */
int goToDestAlgo(binaryTree* current, binaryTreeData destData, binaryTree* log)
{
    //if there is no this node in this tree, return Null
    if(findNodeWithDfsWithLocation(toTreeRoot(current),destData) == NULL)
    {
        logFile("not found!");
        return 0;
    }
    log->data.location[WIDTH] = current->data.location[WIDTH];
    log->data.location[HEIGHT] = current->data.location[HEIGHT];

    //base
    if(current->data.location[WIDTH] == destData.location[WIDTH] && current->data.location[HEIGHT] == destData.location[HEIGHT])
    {
        logFile("found!");
        return 1;
    }
    // if we can find it in current's subtree
    if(findNodeWithDfsWithLocation(current,destData) != NULL)
    {
        if(findNodeWithDfsWithLocation(current->leftChild,destData) != NULL)
        {
            logFile("Go leftChild!");
            log->leftChild = initTreeNode(initTreeData(current->leftChild->data.location[WIDTH],current->leftChild->data.location[HEIGHT]));
            log->leftChild->parent = log;
            
            log->leftChild->data.parentX = current->leftChild->data.parentX;
            log->leftChild->data.parentY = current->leftChild->data.parentY;
            log->leftChild->data.parentDir = current->leftChild->data.parentDir;

            log->data.leftChildDir = current->data.leftChildDir;
            log->data.leftChildX = current->data.leftChildX;
            log->data.leftChildY = current->data.leftChildY;
            
            return goToDestAlgo(current->leftChild, destData, log->leftChild);
        }
        else if(findNodeWithDfsWithLocation(current->rightChild,destData) != NULL)
        {
            logFile("Go rightChild!");
            log->rightChild = initTreeNode(initTreeData(current->rightChild->data.location[WIDTH],current->rightChild->data.location[HEIGHT]));
            log->rightChild->parent = log;

            log->rightChild->data.parentX = current->rightChild->data.parentX;
            log->rightChild->data.parentY = current->rightChild->data.parentY;
            log->rightChild->data.parentDir = current->rightChild->data.parentDir;

            log->data.rightChildDir = current->data.rightChildDir;
            log->data.rightChildX = current->data.rightChildX;
            log->data.rightChildY = current->data.rightChildY;
            return goToDestAlgo(current->rightChild, destData, log->rightChild);
        }
    }
    else
    {
        logFile("Go parent!");
        log->parent = initTreeNode(initTreeData(current->parent->data.location[WIDTH],current->parent->data.location[HEIGHT]));
        if(current->parent->leftChild == current)
        {
            log->parent->leftChild = log;

            log->parent->data.leftChildX = current->parent->data.leftChildX;
            log->parent->data.leftChildY = current->parent->data.leftChildY;
            log->parent->data.leftChildDir = current->parent->data.leftChildDir;
        }
        else if(current->parent->rightChild == current)
        {
            log->parent->rightChild = log;
            
            log->parent->data.rightChildX = current->parent->data.rightChildX;
            log->parent->data.rightChildY = current->parent->data.rightChildY;
            log->parent->data.rightChildDir = current->parent->data.rightChildDir;
        }
        log->data.parentDir = current->data.parentDir;
        log->data.parentX = current->data.parentX;
        log->data.parentY = current->data.parentY;
        return goToDestAlgo(current->parent, destData, log->parent);
    }
    // cannot reach here
    return 0;
}