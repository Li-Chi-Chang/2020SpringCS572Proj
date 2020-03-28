#include "map.h"

int initMap = 0;
int mouseX = entryX, mouseY = entryY, mouseHead = entryHead;

mapNode map[ MAZEHEIGHT ][ MAZEWIDTH ];

/*Small functions, not called by outside*/
int isGoable(mapNode, int);
void clearScreen();
int drawMaze(int, int, int);
int goTop(mapNode);

int readMaze()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    int read;

    fp = fopen(MAZEFILE, "r");
    if (fp == NULL)
    {
        return err(FILEERR);
    }

    while ((read = getline(&line, &len, fp)) != -1) 
    {
        const mapNode init = {.north = 0, .east = 0, .south = 0, .west = 0, .name = ' ', .reserve = 0};
        char *tok = strtok(line,"\t");
        int x,y,i;

        for(i=0; tok != NULL; i++ )
        {
            switch (i)
            {
            case 0:
                y = atoi(tok);
                break;
            case 1:
                x = atoi(tok);
                map[x][y] = init;
                break;
            case 2:
                map[x][y].north = atoi(tok);
                break;
            case 3:
                map[x][y].east = atoi(tok);
                break;
            case 4:
                map[x][y].south = atoi(tok);
                break;
            case 5:
                map[x][y].west = atoi(tok);
                break;
            case 6:
                if(tok[0] >= 49 && tok[0] <= 122)
                    map[x][y].name = tok[0];
            default:
                break;
            }
            tok = strtok(NULL, "\t");
        }
    }

    fclose(fp);
    if (line)
    {
        free(line);
    }

    mouseX = entryX, mouseY = entryY, mouseHead = entryHead;
    initMap = 1;
    drawMaze(mouseX,mouseY, 0);
    return err(0);
}

mapNode getNodeInfo()
{
    int north = map[mouseX][mouseY].north;
    int south = map[mouseX][mouseY].south;
    int east = map[mouseX][mouseY].east;
    int west = map[mouseX][mouseY].west;
    int name = map[mouseX][mouseY].name;

    mapNode NodeFromMouseView;
    switch (mouseHead)
    {
    case NORTH:
        NodeFromMouseView.north = north;
        NodeFromMouseView.south = south;
        NodeFromMouseView.east = east;
        NodeFromMouseView.west = west;
    case SOUTH:
        NodeFromMouseView.north = south;
        NodeFromMouseView.south = north;
        NodeFromMouseView.east = west;
        NodeFromMouseView.west = east;
    case WEST:
        NodeFromMouseView.north = west;
        NodeFromMouseView.south = east;
        NodeFromMouseView.east = north;
        NodeFromMouseView.west = south;
    case EAST:
        NodeFromMouseView.north = east;
        NodeFromMouseView.south = west;
        NodeFromMouseView.east = south;
        NodeFromMouseView.west = north;
    }

    NodeFromMouseView.name = name;
    NodeFromMouseView.reserve = 0;
    return NodeFromMouseView;
}

/**
 * char nextMove
 * return 2: reach the exit
 * return 0: reach to next node
 * return 1: hit the wall
 * */
int goNext()
{
    if(!initMap)
    {
        return err(NOINITERR);
    }

    mapNode infoForWalls = map[mouseX][mouseY];
    return goTop(infoForWalls);
}

int rotate(int direction)
{
    switch (direction)
    {
    case BOTTOM:
        switch (mouseHead)
        {
        case NORTH:
            mouseHead = SOUTH;
            break;
        case SOUTH:
            mouseHead = NORTH;
            break;
        case EAST:
            mouseHead = WEST;
            break;
        case WEST:
            mouseHead = EAST;
            break;
        }
        break;
    case RIGHT:
        switch (mouseHead)
        {
        case NORTH:
            mouseHead = EAST;
            break;
        case SOUTH:
            mouseHead = WEST;
            break;
        case EAST:
            mouseHead = SOUTH;
            break;
        case WEST:
            mouseHead = NORTH;
            break;
        }
        break;
    case LEFT:
        switch (mouseHead)
        {
        case SOUTH:
            mouseHead = EAST;
            break;
        case NORTH:
            mouseHead = WEST;
            break;
        case WEST:
            mouseHead = SOUTH;
            break;
        case EAST:
            mouseHead = NORTH;
            break;
        }
        break;
    }
    return drawMaze(mouseX, mouseY, 1);
}

// Small functions
void clearScreen(int isdelay)
{
    if(isdelay)
    {
        sleep(isdelay);
    }
    printf("\e[1;1H\e[2J");
}

int drawMaze(int locationX, int locationY, int isdelay)
{
    if(!initMap)
    {
        return err(NOINITERR);
    }

    int x,y;
    clearScreen(isdelay);
    for(y = MAZEHEIGHT -1; y >= 0; y--)
    {
        //print the north layer
        if(ISPRINTCORD)
        {
            printf("   ");
        }
        for(x = 0; x<MAZEWIDTH; x++)
        {
            printf("+");
            if(map[x][y].north == WALL)
            {
                if(mouseHead == NORTH && mouseX == x && mouseY == y)
                {
                    printf("--^--");
                }
                else if(mouseHead == SOUTH && mouseX == x && mouseY == y + 1)
                {
                    printf("--v--");
                }
                else
                {
                    printf("-----");
                }
            }
            else if(map[x][y].north == ENTRY)
            {
                if(mouseHead == NORTH && mouseX == x && mouseY == y)
                {
                    printf("-]^[-");
                }
                else if(mouseHead == SOUTH && mouseX == x && mouseY == y + 1)
                {
                    printf("-]v[-");
                }
                else
                {
                    printf("-] [-");
                }
                
            }
            else
            {
                if(mouseHead == NORTH && mouseX == x && mouseY == y)
                {
                    printf("  ^  ");
                }
                else if(mouseHead == SOUTH && mouseX == x && mouseY == y + 1)
                {
                    printf("  v  ");
                }
                else
                {
                    printf("     ");
                }
                
            }
        }
        printf("+\n");
        if(ISPRINTCORD)
        {
            printf("%2d ",y);
        }
        //print the middle layer
        for(x = 0; x<MAZEWIDTH; x++)
        {
            if(map[x][y].west == WALL)
            {
                if(mouseHead == WEST && mouseX == x && mouseY == y)
                {
                    printf("<");
                }
                else if(mouseHead == EAST && mouseX == x - 1 && mouseY == y)
                {
                    printf(">");
                }
                else
                {
                    printf("|");
                }
            }
            else if(map[x][y].west == ENTRY)
            {
                if(mouseHead == WEST && mouseX == x && mouseY == y)
                {
                    printf("<");
                }
                else if(mouseHead == EAST && mouseX == x - 1 && mouseY == y)
                {
                    printf(">");
                }
                else
                {
                    printf("=");
                }
            }
            else
            {
                if(mouseHead == WEST && mouseX == x && mouseY == y)
                {
                    printf("<");
                }
                else if(mouseHead == EAST && mouseX == x - 1 && mouseY == y)
                {
                    printf(">");
                }
                else
                {
                    printf(" ");
                }
            }

            if(locationX == x && locationY == y)
            {
                printf("  %1c  ", '@');
            }
            else if(ISPRINTTRACE && map[x][y].name == '-')
            {
                printf(" -%1c- ", map[x][y].name);
            }
            else
            {
                printf("  %1c  ", map[x][y].name);
            }
        }
        if(map[MAZEWIDTH - 1][y].east == WALL)
        {
            printf("|\n");
        }
        else if(map[MAZEWIDTH -1][y].east == ENTRY)
        {
            printf("=\n");
        }
    }
    //print the bottom layer
    if(ISPRINTCORD)
    {
        printf("   ");
    }
    for(x = 0; x < MAZEWIDTH; x++)
    {
        if(map[x][0].south == WALL)
        {
            printf("+-----");
        }
        else if(map[x][0].south == ENTRY)
        {
            printf("+-] [-");
        }
        else
        {
            printf("+     ");
        }
    }
    printf("+\n");
    if(ISPRINTCORD)
    {
        printf("   ");
        for(x = 0; x < MAZEWIDTH; x++)
        {
            printf("  %2d  ", x);
        }
        printf("\n");
    }
    return err(0);
}

int goTop(mapNode currentNode)
{
    int isdelay = 0;
    switch (mouseHead)
    {
    case NORTH:
        if(currentNode.north == GRID)
        {
            if(ISPRINTTRACE && map[mouseX][mouseY].name == ' ')
            {
                map[mouseX][mouseY].name = '|';
            }
            mouseY = mouseY + 1;
            isdelay = 1;
        }
        drawMaze(mouseX, mouseY,isdelay);
        return (currentNode.north<<8) + currentNode.name;
    case SOUTH:
        if(currentNode.south == GRID)
        {
            if(ISPRINTTRACE && map[mouseX][mouseY].name == ' ')
            {
                map[mouseX][mouseY].name = '|';
            }
            mouseY = mouseY - 1;
            isdelay = 1;
        }
        drawMaze(mouseX, mouseY,isdelay);
        return (currentNode.south<<8) + currentNode.name;
    case WEST:
        if(currentNode.west == GRID)
        {
            if(ISPRINTTRACE && map[mouseX][mouseY].name == ' ')
            {
                map[mouseX][mouseY].name = '-';
            }
            mouseX = mouseX - 1;
            isdelay = 1;
        }
        drawMaze(mouseX, mouseY,isdelay);
        return (currentNode.west<<8) + currentNode.name;
    case EAST:
        if(currentNode.east == GRID)
        {
            if(ISPRINTTRACE && map[mouseX][mouseY].name == ' ')
            {
                map[mouseX][mouseY].name = '-';
            }
            mouseX = mouseX + 1;
            isdelay = 1;
        }
        drawMaze(mouseX, mouseY,isdelay);
        return (currentNode.east<<8) + currentNode.name;
    default:
        return err(COMMANDNOTFOUND);
    }
}
