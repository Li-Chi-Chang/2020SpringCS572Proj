#include "map.h"
#include "stackInt.h"

void init();
void goToTheRight();

int main(void)
{
    init();
    // do the bfs algo
    goToTheRight();
    return 0;
}

void init()
{
    // init
    readMaze();
    initStackInt();
}

/**
 * algo: go until meet branch
 * If meet a brach, go the right side
 * using the bfs algo
 * */
void goToTheRight()
{
    mapNode current = getNodeInfo();
    char mousetop = current.north;
    char mouseleft = current.west;
    char mouseright = current.east;

    if(mousetop == ENTRY || mouseleft == ENTRY || mouseright == ENTRY)
    {
        
    }

    while(rotate(RIGHT)==0);
}