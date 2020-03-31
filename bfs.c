#include "map.h"
#include "singleStackInt.h"
#include "linkedListInt.h"

void init();
int goToNextBranch();
void dfs();

char dfsNode[MAZEHEIGHT * MAZEWIDTH];

int main(void)
{
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
    if(current.south == GRID)
    {
        way++;
    }

    if(way == 3)
    {
        
    }
}

void init()
{
    // init
    readMaze();
    initStackInt();
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
            rotate(RIGHT);
        }
        else if(current.west == GRID)
        {
            rotate(LEFT);
        }
        goNext();
        return goToNextBranch();
    case 2:
        return 0;
    }

    if(current.north == ENTRY)
        ;
    if(current.west == ENTRY)
    {
        rotate(LEFT);
    }
    if(current.east == ENTRY)
    {
        rotate(RIGHT);
    }
    return 1;
}