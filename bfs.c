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

void goToTheRight()
{
    while(goNext()>>8 != WALL);
    rotate(RIGHT);
    if(goNext()>>8 != WALL)
    {
        goToTheRight();
    }
    else
    {
        rotate(LEFT);
        rotate(LEFT);
        if(goNext()>>8 != WALL)
        {
            goToTheRight();
        }
    }
}