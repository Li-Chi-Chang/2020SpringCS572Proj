#include "../map/map.h"
#include "../stack_C/stack.h"
#include "../linkedList_C/linkedList.h"
#include "../binaryTree_C/binaryTree.h"
#include "../queue_C/queue.h"

//basic actions
void turn(char);//turn a direction
void go();//go forward one step
mapNode getRealDirection(mapNode, int);
int getBottom(int);
void printCurrentNode(mapNode, int);

//advenced actions
int goToNextBranch();//go stregth to next fork node
int goToNextBranchWithCost(int);
void faceTo(int);//face to one of NEWS

binaryTree* findNodeWithDfsWithLocation(binaryTree*, binaryTreeData);//input pointer, data, return pointer

void goToDest(binaryTree*, binaryTree*);//from this node to other node in tree, second input is the path
int goToDestAlgo(binaryTree*, binaryTreeData, binaryTree*);//the third input is an output

int calculateCost(binaryTree*);
void integrateCross(binaryTree*);

//algo
void dijkstra();

//others
binaryTreeData initTreeData(int, int);
int printTreeText(binaryTree*,int);
int findEmptyCrossField(binaryTree*);
int findEmptyUnchooseField(binaryTree*);

void printStackText(stackBase*);
stackData popStackWithText(stackBase*);
int pushStackWithText(stackBase*, stackData);

queueData popQueueWithTextOrdered(queueBase*, binaryTree*);
queueData popQueueWithText(queueBase*);
int pushQueueWithText(queueBase*, queueData);
void printQueueText(queueBase*);

char mousehead = NORTH;
int location[2] = {0,0};// 1:x 0:y
binaryTree* tree;
stackBase* stack;
queueBase* queue;

#define PREVIOUSCOST 0
#define PREVIOUSX 1
#define PREVIOUSY 2
int dijkstraTable[MAZEWIDTH][MAZEHEIGHT][3];//0:min path to previous node; 1: previousX; 2: previousY

#define HEIGHT 0
#define WIDTH 1

#define UNDEFINE 99
#define ENTRYWAY 100
#define HITWALL 98

int main(void)
{
    // must init first
    readMaze();
    initErrLog();
    binaryTreeData data = initTreeData(location[WIDTH],location[HEIGHT]);
    data.CostFromStartPoint = 0;
    tree = initTreeNode(data);
    queue = initQueueBase();
    queueData qData;

    qData.locationX = location[WIDTH];
    qData.locationY = location[HEIGHT];

    if(getRealDirection(getNodeInfo(),mousehead).north == GRID)
    {
        qData.travelDir = NORTH;
        pushQueueWithText(queue, qData);
    }
    if(getRealDirection(getNodeInfo(),mousehead).east == GRID)
    {
        qData.travelDir = EAST;
        pushQueueWithText(queue, qData);
    }
    if(getRealDirection(getNodeInfo(),mousehead).south == GRID)
    {
        qData.travelDir = SOUTH;
        pushQueueWithText(queue, qData);
    }
    if(getRealDirection(getNodeInfo(),mousehead).west == GRID)
    {
        qData.travelDir = WEST;
        pushQueueWithText(queue, qData);
    }
    
    dijkstra();
    return 0;
}

void dijkstra()
{
    logFile("In dijkstra function.>>>");
    printQueueText(queue);
    printTreeText(toTreeRoot(tree),1);
    logFile("In dijkstra function.<<<");
    if(getQueueLen(queue)==0)
    {
        logFile("finish search");
        return;
    }
    queueData queuedata = popQueueWithTextOrdered(queue,tree);
    //if the edge is searched?
    if(findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(queuedata.locationX,queuedata.locationY))!=NULL)
    {
        binaryTree *existNode = findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(queuedata.locationX,queuedata.locationY));
        int searched=0;
        int i;
        for(i = 0;i<MAXCROSS;i++)
        {
            if(existNode->cross[i]!=NULL&& existNode->data.crossDir[i]==queuedata.travelDir)
            {
                searched = 1;
                break;
            }
            if(existNode->unchoose[i]!=NULL&& existNode->data.unchooseDir[i]==queuedata.travelDir)
            {
                searched = 1;
                break;
            }
        }
        if(existNode->parent!=NULL&& existNode->data.parentDir==queuedata.travelDir)
            searched=1;

        if(searched)
        {
            logFile("this edge is searched");
            return dijkstra();
        }
    }
    
    // if we are in the node
    if(!(queuedata.locationX == location[WIDTH] && queuedata.locationY == location[HEIGHT]))
    {
        //we are not in the node
        //need to go to other node
        logFile("need to go to other node");
        sprintf(LogFileString,"queuedata.locationX:%d location[WIDTH]:%d\nqueuedata.locationY:%d location[HEIGHT]:%d",queuedata.locationX,location[WIDTH],queuedata.locationY,location[HEIGHT]);
        logFile(LogFileString);
        binaryTree* log = initTreeNode(initTreeData(UNDEFINE,UNDEFINE));
        goToDestAlgo(tree,initTreeData(queuedata.locationX,queuedata.locationY),log);
        logFile("need to go to other node>>>");
        printTreeText(toTreeRoot(log),1);
        logFile("need to go to other node<<<");
        goToDest(tree, log);
        tree = findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(location[WIDTH],location[HEIGHT]));
    }

    // go to this dir find this node
    faceTo(queuedata.travelDir);
    go();
    int cost = goToNextBranchWithCost(1);
    //check if this node is in tree
    binaryTree *crossPoint = findNodeWithDfsWithLocation(toTreeRoot(tree),initTreeData(location[WIDTH],location[HEIGHT]));
    if(crossPoint!=NULL)
    {
        //this is a cross!
        //need to calculate the min path
        
        logFile("do the min path algo");
        sprintf(LogFileString,"the cost of node:(%2d,%2d), cost:%d",location[WIDTH],location[HEIGHT],calculateCost(tree));
        logFile(LogFileString);
        sprintf(LogFileString,"the cost of node:(%2d,%2d), cost:%d",crossPoint->data.location[WIDTH],crossPoint->data.location[HEIGHT],calculateCost(crossPoint));
        logFile(LogFileString);
        logFile("crossPoint:");
        printTreeText(crossPoint,0);
        logFile("tree:");
        printTreeText(tree,0);
        binaryTree *treeParent = tree->parent;
        logFile("treeParent:");
        printTreeText(treeParent,0);
        if(calculateCost(tree)+cost < calculateCost(crossPoint))
        {
            logFile("change the cross!0");
            int unchooseT,unchooseP;
            unchooseT = findEmptyUnchooseField(tree);
            unchooseP = findEmptyUnchooseField(crossPoint);
            tree->unchoose[unchooseT] = crossPoint;
            tree->data.unchooseDir[unchooseT] = queuedata.travelDir;
            tree->data.unchooseX[unchooseT] = crossPoint->data.location[WIDTH];
            tree->data.unchooseY[unchooseT] = crossPoint->data.location[HEIGHT];

            crossPoint->unchoose[unchooseP] = tree;
            crossPoint->data.unchooseDir[unchooseP] = getBottom(mousehead);
            crossPoint->data.unchooseX[unchooseP] = tree->data.location[WIDTH];
            crossPoint->data.unchooseY[unchooseP] = tree->data.location[HEIGHT];
        }
        else
        {
            int UnchooseTP = findEmptyUnchooseField(treeParent);
            int i;
            for(i = 0; i<MAXCROSS;i++)
            {
                if(treeParent->cross[i]!=NULL&&treeParent->data.crossX[i] == tree->data.location[WIDTH]&&treeParent->data.crossY[i] == tree->data.location[HEIGHT])
                {
                    break;
                }
            }
            treeParent->unchoose[UnchooseTP] = treeParent->cross[i];
            treeParent->data.unchooseDir[UnchooseTP] = treeParent->data.crossDir[i];
            treeParent->data.unchooseX[UnchooseTP] = treeParent->data.crossX[i];
            treeParent->data.unchooseY[UnchooseTP] = treeParent->data.crossY[i];

            int Unchoose = findEmptyUnchooseField(tree);
            tree->unchoose[Unchoose] = tree->parent;
            tree->data.unchooseDir[Unchoose] = tree->data.parentDir;
            tree->data.unchooseX[Unchoose] = tree->data.parentX;
            tree->data.unchooseY[Unchoose] = tree->data.parentY;
            logFile("");
            printTreeText(tree,0);
            tree->parent = crossPoint;
            tree->data.parentDir = queuedata.travelDir;
            tree->data.parentX = crossPoint->data.location[WIDTH];
            tree->data.parentY = crossPoint->data.location[HEIGHT];
            
            int crossP = findEmptyCrossField(crossPoint);
            crossPoint->cross[crossP] = tree;
            crossPoint->data.crossDir[crossP] = getBottom(mousehead);
            crossPoint->data.crossX[crossP] = tree->data.location[WIDTH];
            crossPoint->data.crossY[crossP] = tree->data.location[HEIGHT];

            logFile("");
            printTreeText(tree,0);
            treeParent->cross[i] = NULL;
            treeParent->data.crossDir[i]=UNDEFINE;
            treeParent->data.crossX[i]=UNDEFINE;
            treeParent->data.crossY[i]=UNDEFINE;

            tree->data.CostFromStartPoint = cost;
            integrateCross(treeParent);
            logFile("change the cross!1");
            printTreeText(toTreeRoot(tree),1);
            tree = crossPoint;
        }
    }
    else
    {
        //new node, connect this node
        int cross = findEmptyCrossField(tree);
        tree->cross[cross] = initTreeNode(initTreeData(location[WIDTH],location[HEIGHT]));
        tree->data.crossDir[cross] = queuedata.travelDir;
        tree->data.crossX[cross] = location[WIDTH];
        tree->data.crossY[cross] = location[HEIGHT];

        tree->cross[cross]->data.CostFromStartPoint = cost;
        tree->cross[cross]->parent = tree;
        tree->cross[cross]->data.parentDir = getBottom(mousehead);
        tree->cross[cross]->data.parentX = tree->data.location[WIDTH];
        tree->cross[cross]->data.parentY = tree->data.location[HEIGHT];

        //push new paths to queue
        queueData qData;
        qData.locationX = location[WIDTH];
        qData.locationY = location[HEIGHT];
        logFile("push new paths to queue");
        printCurrentNode(getNodeInfo(),mousehead);
        if(getRealDirection(getNodeInfo(),mousehead).north == GRID && tree->cross[cross]->data.parentDir != NORTH)
        {
            qData.travelDir = NORTH;
            pushQueueWithText(queue, qData);
        }
        if(getRealDirection(getNodeInfo(),mousehead).east == GRID && tree->cross[cross]->data.parentDir != EAST)
        {
            qData.travelDir = EAST;
            pushQueueWithText(queue, qData);
        }
        if(getRealDirection(getNodeInfo(),mousehead).south == GRID && tree->cross[cross]->data.parentDir != SOUTH)
        {
            qData.travelDir = SOUTH;
            pushQueueWithText(queue, qData);
        }
        if(getRealDirection(getNodeInfo(),mousehead).west == GRID && tree->cross[cross]->data.parentDir != WEST)
        {
            qData.travelDir = WEST;
            pushQueueWithText(queue, qData);
        }

        tree = tree->cross[cross];
    }

    logFile("out dijkstra>>>");
    printTreeText(toTreeRoot(tree),1);
    logFile("out dijkstra<<<");
    return dijkstra();
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
 * return 2 hit the name node
 * algo: go until meet branch
 * If meet a brach, go the right side
 * using the bfs algo
 * */
int goToNextBranch()
{
    mapNode current = getNodeInfo();

    //if hit name node return
    if(current.name>='A'&&current.name<='z')
    {
        return 2;
    }
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
    data.CostFromStartPoint = 1e9;
    data.parentX = UNDEFINE;
    data.parentY = UNDEFINE;
    data.parentDir = UNDEFINE;
    data.leftChildX = UNDEFINE;
    data.leftChildY = UNDEFINE;
    data.leftChildDir = UNDEFINE;
    data.rightChildX = UNDEFINE;
    data.rightChildY = UNDEFINE;
    data.rightChildDir = UNDEFINE;
    int i;
    for(i=0;i<MAXCROSS;i++)
    {
        data.crossX[MAXCROSS] = UNDEFINE;
        data.crossY[MAXCROSS] = UNDEFINE;
        data.crossDir[MAXCROSS] = UNDEFINE;
        data.unchooseDir[MAXCROSS] = UNDEFINE;
        data.unchooseY[MAXCROSS] = UNDEFINE;
        data.unchooseX[MAXCROSS] = UNDEFINE;
    }
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
    int i;
    for(i=0;i<MAXCROSS && root->cross[i]!=NULL;i++)
    {
        nextLevel = root->cross[i];
        returnInt = findNodeWithDfsWithLocation(nextLevel, data);
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

void goToDest(binaryTree* treeNode, binaryTree* path)
{
    int parentX = UNDEFINE, parentY = UNDEFINE;
    do{
        sprintf(LogFileString,"location: x:%d, y:%d", location[WIDTH],location[HEIGHT]);
        logFile(LogFileString);
        sprintf(LogFileString,"parentX:%d, parentY:%d",parentX,parentY);
        logFile(LogFileString);
        printTreeText(path,0);

        int cross;
        for(cross=0;cross<MAXCROSS;cross++)
        {
            if(path->cross[cross]!=NULL && (path->data.crossX[cross]!=parentX || path->data.crossY[cross]!=parentY))
            {
                break;
            }
        }
        sprintf(LogFileString,"cross: %d", cross);
        logFile(LogFileString);

        if(path->parent!=NULL && (path->data.parentX!=parentX || path->data.parentY!=parentY))
        {
            //set mouse
            faceTo(treeNode->data.parentDir);
            go();
            goToNextBranch();
            //set tree pointer
            parentX = path->data.location[WIDTH];
            parentY = path->data.location[HEIGHT];
            path = path->parent;
            treeNode = treeNode->parent;
            sprintf(LogFileString,"location1: x:%d, y:%d", location[WIDTH],location[HEIGHT]);
            logFile(LogFileString);
        }
        else if(cross!=MAXCROSS)
        {
            faceTo(treeNode->data.crossDir[cross]);
            go();
            goToNextBranch();
            //set tree pointer
            parentX = path->data.location[WIDTH];
            parentY = path->data.location[HEIGHT];
            path = path->cross[cross];
            treeNode = treeNode->cross[cross];
            sprintf(LogFileString,"location2: x:%d, y:%d", location[WIDTH],location[HEIGHT]);
            logFile(LogFileString);
        }
        else
        {
            break;
        }
        logFile("in while<<");
    }while(1);
    logFile("go to dest");
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
        int i;
        for(i=0;i<MAXCROSS && current->cross[i]!=NULL;i++)
        {
            if(findNodeWithDfsWithLocation(current->cross[i], destData) != NULL)
            {
                sprintf(LogFileString,"go to cross:%d",i);
                logFile(LogFileString);

                log->cross[i] = initTreeNode(initTreeData(current->cross[i]->data.location[WIDTH],current->cross[i]->data.location[HEIGHT]));
                log->cross[i]->parent = log;
                
                log->cross[i]->data.parentX = current->cross[i]->data.parentX;
                log->cross[i]->data.parentY = current->cross[i]->data.parentY;
                log->cross[i]->data.parentDir = current->cross[i]->data.parentDir;

                log->data.crossDir[i] = current->data.crossDir[i];
                log->data.crossX[i] = current->data.crossX[i];
                log->data.crossY[i] = current->data.crossY[i];
                return goToDestAlgo(current->cross[i], destData, log->cross[i]);
            }
        }
    }
    else
    {
        logFile("Go parent!");
        log->parent = initTreeNode(initTreeData(current->parent->data.location[WIDTH],current->parent->data.location[HEIGHT]));
        int cross;
        for(cross = 0; cross<MAXCROSS && current->parent->cross[cross]!=NULL; cross++)
        {
            if(current->parent->cross[cross] == current)
            {
                log->parent->cross[cross] = log;

                log->parent->data.crossX[cross] = current->parent->data.crossX[cross];
                log->parent->data.crossY[cross] = current->parent->data.crossY[cross];
                log->parent->data.crossDir[cross] = current->parent->data.crossDir[cross];

                log->data.parentDir = current->data.parentDir;
                log->data.parentX = current->data.parentX;
                log->data.parentY = current->data.parentY;
                break;
            }
        }
        return goToDestAlgo(current->parent, destData, log->parent);
    }
    // cannot reach here
    return 0;
}



/**
 * Queue Functions
 */
int pushQueueWithText(queueBase* base, queueData data)
{
    sprintf(LogFileString,"pushQ\t(%2d,%2d),dir:%d\n", data.locationX,data.locationY,data.travelDir);
    logFile(LogFileString);
    return pushQueue(base, data);
}

queueData popQueueWithText(queueBase* base)
{
    queueData data = popQueue(base);
    sprintf(LogFileString,"popQ\t(%2d,%2d),dir:%d\n", data.locationX,data.locationY,data.travelDir);
    logFile(LogFileString);
    return data;
}

queueData popQueueWithTextOrdered(queueBase* base, binaryTree* node)
{
    //todo!
    queueData data = popQueue(base);
    sprintf(LogFileString,"popQ\t(%2d,%2d),dir:%d\n", data.locationX,data.locationY,data.travelDir);
    logFile(LogFileString);
    return data;
}

void printQueueText(queueBase* base)
{
    sprintf(LogFileString, "Print Queue(%d):",getQueueLen(base));
    logFile(LogFileString);
    logFile("top");
    queueNode* pointer = base->next;
    int i;
    for(i = getQueueLen(base); i>0; i--)
    {
        sprintf(LogFileString, "\t(%d,%d),dir:%d",pointer->data.locationX,pointer->data.locationY,pointer->data.travelDir);
        logFile(LogFileString);
        pointer = pointer->next;
    }
    logFile("tail\n");
}

/**
 * Stack Functions
 */
int pushStackWithText(stackBase* base, stackData data)
{
    sprintf(LogFileString,"pushS\t(%2d,%2d),dir:%d\n", data.locationX,data.locationY,data.travelDir);
    logFile(LogFileString);
    return pushStack(base, data);
}

stackData popStackWithText(stackBase* base)
{
    stackData data = popStack(base);
    sprintf(LogFileString,"popS\t(%2d,%2d),dir:%d\n", data.locationX,data.locationY,data.travelDir);
    logFile(LogFileString);
    return data;
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
        sprintf(LogFileString, "\t(%d,%d),dir:%d",pointer->data.locationX,pointer->data.locationY,pointer->data.travelDir);
        logFile(LogFileString);
        pointer = pointer->next;
    }
    logFile("top\n");
}

/**
 * Tree Functions
 */
int findEmptyCrossField(binaryTree* root)
{
    int i;
    for(i=0;i<MAXCROSS;i++)
    {
        if(root->cross[i]==NULL)
        {
            return i;
        }
    }
    return MAXCROSS;
}

int findEmptyUnchooseField(binaryTree* root)
{
    int i;
    for(i=0;i<MAXCROSS;i++)
    {
        if(root->unchoose[i]==NULL)
        {
            return i;
        }
    }
    return MAXCROSS;
}

int printTreeText(binaryTree* root, int recurrsiveOrNot)
{
    sprintf(LogFileString,"this node:\t(%2d,%2d)\tcost: %d", root->data.location[WIDTH],root->data.location[HEIGHT],root->data.CostFromStartPoint);
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
    int i=0;
    while(i<MAXCROSS && root->cross[i]!=NULL)
    {
        sprintf(LogFileString,"\tcross[%d]:    (%2d,%2d)   dir:%d",i,root->data.crossX[i],root->data.crossY[i],root->data.crossDir[i]);
        logFile(LogFileString);
        i++;
    }

    i=0;
    while(i<MAXCROSS && root->unchoose[i]!=NULL)
    {
        sprintf(LogFileString,"\tunchoose[%d]:    (%2d,%2d)   dir:%d",i,root->data.unchooseX[i],root->data.unchooseY[i],root->data.unchooseDir[i]);
        logFile(LogFileString);
        i++;
    }
    if(recurrsiveOrNot)
    {
        int j=0;
        while(i<MAXCROSS && root->cross[j]!=NULL)
        {
            printTreeText(root->cross[j], 1);
            j++;
        }
    }
    return 0;
}

int goToNextBranchWithCost(int cost)
{
    mapNode current = getNodeInfo();

    //if hit name node return
    if(current.name>='A'&&current.name<='z')
    {
        return cost;
    }
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
    case 2:
        //hit a wall, or 3 way node
        return cost;
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
        return goToNextBranchWithCost(cost)+1;
    }
    return err(COMMANDNOTFOUND);
}

int calculateCost(binaryTree* node)
{
    if(node->data.CostFromStartPoint == 0)
        return 0;
    return calculateCost(node->parent) + node->data.CostFromStartPoint;
}

void integrateCross(binaryTree* node)
{
    binaryTree* temp[MAXCROSS];
    int cross,i = 0;
    for (cross = 0; cross < MAXCROSS; cross++)
    {
        if(node->cross[cross]!=NULL)
        {
            temp[i] = node->cross[cross];
            node->cross[cross] = NULL;
            i++;
        }
    }
    for(cross = 0; cross < i; cross++)
    {
        node->cross[cross] = temp[cross];
    }
}