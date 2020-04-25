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

    sprintf(string, "init location: x:%d, y:%d", location[WIDTH],location[HEIGHT]);
    logFile(string);
    sprintf(string, "data location: x:%d, y:%d", data.locationX,data.locationY);
    logFile(string);
    switch(current.east)
    {
    case GRID:
        data.travelDir = EAST;
        push(stack,data);
        break;
    }
    switch(current.west)
    {
    case GRID:
        data.travelDir = WEST;
        push(stack,data);
        break;
    }
    switch(current.north)
    {
    case GRID:
        data.travelDir = NORTH;
        push(stack,data);
        break;
    }
    switch(current.south)
    {
    case GRID:
        data.travelDir = SOUTH;
        push(stack,data);
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
    binaryTreeData data = initTreeData();
    tree = initTreeNode(data);
    stack = initStackBase();
}


void dfs(int parentX, int parentY, int exitDir)
{
    if(tree->data.location[WIDTH] != location[WIDTH] || tree->data.location[HEIGHT] != location[HEIGHT])
    {
        printf("tree->data.location[WIDTH]:%d, location[WIDTH]:%d\ntree->data.location[HEIGHT]:%d, location[HEIGHT]:%d\n",tree->data.location[WIDTH],location[WIDTH], tree->data.location[HEIGHT], location[HEIGHT]);
        printTreeText(toTreeRoot(tree),1);
        err(COMMANDNOTFOUND);
    }
        

    logFile("\nin dfs");
    sprintf(string,"nodeinfo:\tX:%d,Y:%d",location[WIDTH],location[HEIGHT]);
    logFile(string);
    printStackText(stack);

    //if the stack is empty, finish traveling
    if(getStackLen(stack) == 0)
        return;
    stackData stackinfo = pop(stack);
    sprintf(string,"pop\tX:%d,Y:%d,Dir:%d",stackinfo.locationX,stackinfo.locationY,stackinfo.travelDir);
    logFile(string);

    //if the target node is the current node, operate the command in stack
    if(location[WIDTH] == stackinfo.locationX && stackinfo.locationY == location[HEIGHT])
    {
        binaryTree *pointer = toTreeRoot(tree);
        binaryTreeData treedata = initTreeData();
        treedata.location[WIDTH] = location[WIDTH];
        treedata.location[HEIGHT] = location[HEIGHT];
        printTreeText(toTreeRoot(tree),1);
        //if we can find this node in tree, it means that it is searched
        if(findNodeWithDfsWithLocation(pointer,treedata) != NULL)
        {
            pointer = findNodeWithDfsWithLocation(pointer,treedata);
            if(pointer->cross != NULL)
            {
                //if the path will go is the cross path, dont go, or you will fall in to a loop
                if(pointer->data.crossDir == stackinfo.travelDir)
                {
                    logFile("hit cross and return!");
                    dfs(location[WIDTH],location[HEIGHT],mousehead);
                }
            }
            faceTo(stackinfo.travelDir);
            go();
            int result = goToNextBranch();
            switch (result)
            {
            case HITWALL:
                //hit a wall, so back to original node
                turn(LEFT);
                turn(LEFT);
                goToNextBranch();
                if(pointer->leftChild == NULL)
                {
                    treedata.parentDir = getBottom(mousehead);
                    treedata.parentX = parentX;
                    treedata.parentY = parentY;
                    pointer->leftChild = initTreeNode(treedata);
                    pointer->leftChild->parent = pointer;
                    pointer->leftChild->data.location[WIDTH] = HITWALL;
                    pointer->leftChild->data.location[HEIGHT] = HITWALL;
                    pointer->data.leftChildX = HITWALL;
                    pointer->data.leftChildY = HITWALL;
                    pointer->data.leftChildDir = exitDir;
                }
                else
                {
                    treedata.parentDir = getBottom(mousehead);
                    treedata.parentX = parentX;
                    treedata.parentY = parentY;
                    pointer->rightChild = initTreeNode(treedata);
                    pointer->rightChild->parent = pointer;
                    pointer->rightChild->data.location[WIDTH] = HITWALL;
                    pointer->rightChild->data.location[HEIGHT] = HITWALL;
                    pointer->data.rightChildX = HITWALL;
                    pointer->data.rightChildY = HITWALL;
                    pointer->data.rightChildDir = exitDir;
                }
                dfs(location[WIDTH],location[HEIGHT],getBottom(mousehead));
                break;
            case 1:
            case 0:
                if(1)
                {
                    //need to check if this node in tree, if yes, add this node to cross
                    binaryTreeData data = initTreeData();
                    data.location[WIDTH] = location[WIDTH];
                    data.location[HEIGHT] = location[HEIGHT];
                    if(findNodeWithDfsWithLocation(toTreeRoot(tree),data)!=NULL)
                    {
                        logFile("in cross function");
                        binaryTree *existNode = findNodeWithDfsWithLocation(toTreeRoot(tree),data);
                        if(existNode->cross!=NULL && existNode->cross != pointer)
                        {
                            logFile("Cross number is not enough!!!!");
                            exit(1);
                        }
                        // meet the same cross from the other side
                        else if(existNode->cross!=NULL && existNode->cross == pointer)
                        {
                            logFile("meet the same cross from the other side");
                            pointer = pointer->cross;
                            tree = pointer;
                            dfs(location[WIDTH],location[HEIGHT],mousehead);
                            break;
                        }
                        existNode->cross = pointer;
                        pointer->cross = existNode;
                        existNode->data.crossX = pointer->data.location[WIDTH];
                        existNode->data.crossY = pointer->data.location[HEIGHT];
                        existNode->data.crossDir = getBottom(mousehead);
                        pointer->data.crossX = existNode->data.location[WIDTH];
                        pointer->data.crossY = existNode->data.location[HEIGHT];
                        pointer->data.crossDir = exitDir;
                        turn(LEFT);
                        turn(LEFT);
                        go();
                        goToNextBranch();
                        dfs(location[WIDTH],location[HEIGHT],mousehead);
                        break;
                    }
                }
                if(tree->leftChild == NULL)
                {
                    treedata.parentDir = getBottom(mousehead);
                    treedata.parentX = parentX;
                    treedata.parentY = parentY;
                    pointer->leftChild = initTreeNode(treedata);
                    pointer->leftChild->parent = pointer;
                    pointer->leftChild->data.location[WIDTH] = location[WIDTH];
                    pointer->leftChild->data.location[HEIGHT] = location[HEIGHT];
                    pointer->data.leftChildDir = exitDir;
                    pointer->data.leftChildX = location[WIDTH];
                    pointer->data.leftChildY = location[HEIGHT];
                    tree = pointer->leftChild;
                    pointer = pointer->leftChild;
                }
                else if(tree->rightChild == NULL)
                {
                    treedata.parentDir = getBottom(mousehead);
                    treedata.parentX = parentX;
                    treedata.parentY = parentY;
                    pointer->rightChild = initTreeNode(treedata);
                    pointer->rightChild->parent = pointer;
                    pointer->rightChild->data.location[WIDTH] = location[WIDTH];
                    pointer->rightChild->data.location[HEIGHT] = location[HEIGHT];
                    pointer->data.rightChildDir = exitDir;
                    pointer->data.rightChildX = location[WIDTH];
                    pointer->data.rightChildY = location[HEIGHT];
                    tree = pointer->rightChild;
                    pointer = pointer->rightChild;
                }
                // if we find a node with 0 empty child branch
                else
                {
                    printf("meet here\n");
                    exit(1);
                }
                if(1)
                {
                    mapNode current = getRealDirection(getNodeInfo(),mousehead);
                    stackData data;
                    data.locationX = location[WIDTH];
                    data.locationY = location[HEIGHT];
                    sprintf(string,"in switch X: %d, Y: %d",location[WIDTH],location[HEIGHT]);
                    logFile(string);
                    sprintf(string,"current north: %d, s: %d, w: %d, e :%d",current.north,current.south,current.west,current.east);
                    logFile(string);
                    sprintf(string,"getNodeInfo north: %d, s: %d, w: %d, e :%d\t head: %d",getNodeInfo().north,getNodeInfo().south,getNodeInfo().west,getNodeInfo().east,mousehead);
                    logFile(string);
                    switch(current.east)
                    {
                    case GRID:
                        if(pointer->data.parentDir == EAST || pointer->data.rightChildDir == EAST || pointer->data.leftChildDir == EAST)
                            break;
                        data.travelDir = EAST;
                        push(stack,data);
                        sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                        logFile(string);
                        break;
                    }
                    switch(current.west)
                    {
                    case GRID:
                        if(pointer->data.parentDir == WEST || pointer->data.rightChildDir == WEST || pointer->data.leftChildDir == WEST)
                            break;
                        data.travelDir = WEST;
                        push(stack,data);
                        sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                        logFile(string);
                        break;
                    }
                    switch(current.north)
                    {
                    case GRID:
                        if(pointer->data.parentDir == NORTH || pointer->data.rightChildDir == NORTH || pointer->data.leftChildDir == NORTH)
                            break;
                        data.travelDir = NORTH;
                        push(stack,data);
                        sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                        logFile(string);
                        break;
                    }
                    switch(current.south)
                    {
                    case GRID:
                        if(pointer->data.parentDir == SOUTH || pointer->data.rightChildDir == SOUTH || pointer->data.leftChildDir == SOUTH)
                            break;
                        data.travelDir = SOUTH;
                        push(stack,data);
                        sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                        logFile(string);
                        break;
                    }
                    dfs(location[WIDTH],location[HEIGHT],getBottom(mousehead));
                }
            }
        }
        //if we can not find this node in tree, it means that we can build a node for it
        else
        {
            //not possible here
        }
    }
    else
    {
        //if the node is not the node in pop, back to that node
        binaryTree* pointer = tree;
        binaryTreeData treedata = initTreeData();
        treedata.location[WIDTH] = stackinfo.locationX;
        treedata.location[HEIGHT] = stackinfo.locationY;
        int trueorNot = findNodeWithDfsWithLocation(pointer,treedata)!=NULL;
        sprintf(string, "stackinfo x:%d y:%d\npointer: x:%d, y:%d\nstatement: %d",stackinfo.locationX,stackinfo.locationY, pointer->data.location[WIDTH],pointer->data.location[HEIGHT], trueorNot);
        logFile(string);
        do
        {
            logFile("in do while");
            sprintf(string, "location x:%d y:%d",location[WIDTH],location[HEIGHT]);
            logFile(string);
            printTreeText(toTreeRoot(tree),1);
            faceTo(pointer->data.parentDir);
            sprintf(string, "parentDir: %d",pointer->data.parentDir);
            logFile(string);
            go();
            goToNextBranch();
            pointer = pointer->parent;
        }while(findNodeWithDfsWithLocation(pointer,treedata) == NULL && (pointer->data.location[WIDTH] != stackinfo.locationX || pointer->data.location[HEIGHT] != stackinfo.locationY));
        logFile("out of while");
        sprintf(string, "location x:%d y:%d",location[WIDTH],location[HEIGHT]);
        logFile(string);
        if(pointer==NULL)
        {
            err(COMMANDNOTFOUND);
        }
        faceTo(stackinfo.travelDir);
        go();
        int result = goToNextBranch();
        pointer = tree;
        switch (result)
        {
        case HITWALL:
            //hit a wall, so back to original node
            turn(LEFT);
            turn(LEFT);
            goToNextBranch();
            if(pointer->leftChild == NULL)
            {
                treedata.parentDir = getBottom(mousehead);
                treedata.parentX = parentX;
                treedata.parentY = parentY;
                pointer->leftChild = initTreeNode(treedata);
                pointer->leftChild->parent = pointer;
                pointer->leftChild->data.location[WIDTH] = HITWALL;
                pointer->leftChild->data.location[HEIGHT] = HITWALL;
                pointer->data.leftChildX = HITWALL;
                pointer->data.leftChildY = HITWALL;
                pointer->data.leftChildDir = exitDir;
                tree = pointer;
            }
            else
            {
                treedata.parentDir = getBottom(mousehead);
                treedata.parentX = parentX;
                treedata.parentY = parentY;
                pointer->rightChild = initTreeNode(treedata);
                pointer->rightChild->parent = pointer;
                pointer->rightChild->data.location[WIDTH] = HITWALL;
                pointer->rightChild->data.location[HEIGHT] = HITWALL;
                pointer->data.rightChildX = HITWALL;
                pointer->data.rightChildY = HITWALL;
                pointer->data.rightChildDir = exitDir;
                tree = pointer;
            }
            break;
        case 1:
            //if i hit an entry
            if(!ISTRAVELALL)
            {

            }
        case 0:
            if(tree->leftChild == NULL)
            {
                logFile("add node at left");
                treedata.parentDir = getBottom(mousehead);
                treedata.parentX = parentX;
                treedata.parentY = parentY;
                pointer->leftChild = initTreeNode(treedata);
                pointer->leftChild->parent = pointer;
                pointer->leftChild->data.location[WIDTH] = location[WIDTH];
                pointer->leftChild->data.location[HEIGHT] = location[HEIGHT];
                pointer->data.leftChildX = location[WIDTH];
                pointer->data.leftChildY = location[HEIGHT];
                pointer->data.leftChildDir = exitDir;
                tree = pointer->leftChild;
                pointer = tree;
            }
            else
            {
                logFile("add node at right");
                treedata.parentDir = getBottom(mousehead);
                treedata.parentX = parentX;
                treedata.parentY = parentY;
                pointer->rightChild = initTreeNode(treedata);
                pointer->rightChild->parent = pointer;
                pointer->rightChild->data.location[WIDTH] = location[WIDTH];
                pointer->rightChild->data.location[HEIGHT] = location[HEIGHT];
                pointer->data.rightChildX = location[WIDTH];
                pointer->data.rightChildY = location[HEIGHT];
                pointer->data.rightChildDir = exitDir;
                tree = pointer->rightChild;
                pointer = tree;
            }
            if(1)
            {
                mapNode current = getRealDirection(getNodeInfo(),mousehead);
                stackData data;
                data.locationX = location[WIDTH];
                data.locationY = location[HEIGHT];
                sprintf(string,"in switch1 X: %d, Y: %d",location[WIDTH],location[HEIGHT]);
                logFile(string);
                sprintf(string,"current north: %d, s: %d, w: %d, e :%d",current.north,current.south,current.west,current.east);
                logFile(string);
                sprintf(string,"getNodeInfo north: %d, s: %d, w: %d, e :%d\t head: %d",getNodeInfo().north,getNodeInfo().south,getNodeInfo().west,getNodeInfo().east,mousehead);
                logFile(string);
                switch(current.east)
                {
                case GRID:
                    if(pointer->data.parentDir == EAST || pointer->data.rightChildDir == EAST || pointer->data.leftChildDir == EAST)
                        break;
                    data.travelDir = EAST;
                    push(stack,data);
                    sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                    logFile(string);
                    break;
                }
                switch(current.west)
                {
                case GRID:
                    if(pointer->data.parentDir == WEST || pointer->data.rightChildDir == WEST || pointer->data.leftChildDir == WEST)
                        break;
                    data.travelDir = WEST;
                    push(stack,data);
                    sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                    logFile(string);
                    break;
                }
                switch(current.north)
                {
                case GRID:
                    if(pointer->data.parentDir == NORTH || pointer->data.rightChildDir == NORTH || pointer->data.leftChildDir == NORTH)
                        break;
                    data.travelDir = NORTH;
                    push(stack,data);
                    sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                    logFile(string);
                    break;
                }
                switch(current.south)
                {
                case GRID:
                    if(pointer->data.parentDir == SOUTH || pointer->data.rightChildDir == SOUTH || pointer->data.leftChildDir == SOUTH)
                        break;
                    data.travelDir = SOUTH;
                    push(stack,data);
                    sprintf(string,"push\tX:%d,Y:%d,Dir:%d",data.locationX,data.locationY,data.travelDir);
                    logFile(string);
                    break;
                }
                dfs(location[WIDTH],location[HEIGHT],getBottom(mousehead));
            }
        }
    }
    
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
    if(current.north == GRID || current.north == ENTRY)
    {
        way++;
        if(current.north == ENTRY)
            entry++;
    }
    if(current.west == GRID|| current.west == ENTRY)
    {
        way++;
        if(current.west == ENTRY)
            entry++;
    }
    if(current.east == GRID|| current.east == ENTRY)
    {
        way++;
        if(current.east == ENTRY)
            entry++;
    }
    if(current.south == ENTRY)
        entry++;
    
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
        location[HEIGHT]++;
        break;
    case SOUTH:
        location[HEIGHT]--;
        break;
    case EAST:
        location[WIDTH]++;
        break;
    case WEST:
        location[WIDTH]--;
        break;
    }

    goNext();
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
        RealDir.north = fromMouseView.west;
        RealDir.south = fromMouseView.east;
        RealDir.east = fromMouseView.north;
        RealDir.west = fromMouseView.south;
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

binaryTreeData initTreeData()
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
    data.location[0] = 0;
    data.location[1] = 0;
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
    

    sprintf(string,"this node: x:%d, y:%d", root->data.location[WIDTH],root->data.location[HEIGHT]);
    logFile(string);

    if(root->parent==NULL)
    {
        sprintf(string,"\tparent: NULL");
        logFile(string);
    }
    else
    {
        sprintf(string,"\tparent: (%d,%d)\t dir:%d",root->data.parentX,root->data.parentY, root->data.parentDir);
        logFile(string);
    }
    if(root->leftChild==NULL)
    {
        sprintf(string,"\tleftChild: NULL");
        logFile(string);
    }
    else
    {
        sprintf(string,"\tleftChild: (%d,%d)",root->data.leftChildX,root->data.leftChildY);
        logFile(string);
    }
    if(root->rightChild==NULL)
    {
        sprintf(string,"\trightChild: NULL");
        logFile(string);
    }
    else
    {
        sprintf(string,"\trightChild: (%d,%d)",root->data.rightChildX,root->data.rightChildY);
        logFile(string);
    }
    if(root->cross!=NULL)
    {
        sprintf(string,"\tcross: (%d,%d)",root->data.crossX,root->data.crossY);
        logFile(string);
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
    sprintf(string, "Print Stack(%d):",getStackLen(base));
    logFile(string);
    logFile("base");
    stackNode* pointer = base->next;
    int i;
    for(i = getStackLen(base); i>0; i--)
    {
        sprintf(string, "(%d,%d),dir:%d",pointer->data.locationX,pointer->data.locationY,pointer->data.travelDir);
        logFile(string);
        pointer = pointer->next;
    }
    logFile("top\n");
}