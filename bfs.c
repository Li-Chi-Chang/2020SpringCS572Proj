#include "map.h"
#include "stack_C/stack.h"
#include "linkedList_C/linkedList.h"
#include "binaryTree_C/binaryTree.h"

//basic actions
void turn(char);//turn a direction
void go();//go forward one step

//advenced actions
int goToNextBranch();//go stregth to next fork node
void chooseNextWay();//choose a way to go at a fork node
void faceTo(int);//face to one of NEWS

//algo
void dfs();

//others
void init();

char mousehead = NORTH;
int location[2] = {0,0};// 1:x 0:y
stackBase* forkNodeStack;

#define HEIGHT 0
#define WIDTH 1

#define TRAVELED 3

int main(void)
{
    // must init first
    init();
    // do the bfs algo
    dfs();
    return 0;
}

void dfs()
{
    mapNode current = getNodeInfo();
    int way = 0;
    if(current.north == GRID)
    {
        way++;
    }
    if(current.west == GRID)
    {
        way++;
    }
    if(current.east == GRID)
    {
        way++;
    }
    if(current.south == GRID || current.south == ENTRY)//for A
    {
        way++;
    }

    switch(way)
    {
    case 0:
        // this is 4-wall node
        break;
    case 1:
        // end node
        break;
        //backToPreviousFork();
    case 2:
        // tunnel node
        goToNextBranch();
        dfs();
        break;
    case 3:
        // fork node
        chooseNextWay();
        dfs();
        break;
    case 4:
        // flat node
    default:
        break;
    }


}

void init()
{
    // init
    readMaze();
    forkNodeStack = initStackInt();
    initErrLog();
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
    if(current.north == GRID)
    {
        way++;
    }
    if(current.west == GRID)
    {
        way++;
    }
    if(current.east == GRID)
    {
        way++;
    }
    
    switch (way)
    {
    case 0:
        //back to previous branch
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
        return 0;
    }

    if(current.north == ENTRY)
        ;
    if(current.west == ENTRY)
    {
        turn(LEFT);
    }
    if(current.east == ENTRY)
    {
        turn(RIGHT);
    }
    return 1;
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

void chooseNextWay()
{
    mapNode current = getNodeInfo();
    
    unsigned int north = current.north;
    unsigned int east = current.east;
    unsigned int west = current.west;

    mapNode RealDir;
    switch (mousehead)
    {
    case NORTH:
        RealDir.north = north;
        RealDir.south = TRAVELED;
        RealDir.east = east;
        RealDir.west = west;
        break;
    case SOUTH:
        RealDir.north = TRAVELED;
        RealDir.south = north;
        RealDir.east = west;
        RealDir.west = east;
        break;
    case WEST:
        RealDir.north = west;
        RealDir.south = east;
        RealDir.east = north;
        RealDir.west = TRAVELED;
        break;
    case EAST:
        RealDir.north = east;
        RealDir.south = west;
        RealDir.east = TRAVELED;
        RealDir.west = north;
        break;
    }

    char dir[4] = {RealDir.north,RealDir.south,RealDir.east,RealDir.west};//north,south,east,west

    int i = 0;
    for(i = 0; i < 4; i++)
    {
        if(dir[i] == 0)
        {
            switch (i)
            {
            case 0:
                faceTo(NORTH);
                break;
            case 1:
                faceTo(SOUTH);
                break;
            case 2:
                faceTo(EAST);
                break;
            case 3:
                faceTo(WEST);
                break;
            default:
                break;
            }
            go();
            goToNextBranch();
            return;
        }
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