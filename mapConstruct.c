#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAZEFILE "mapWithName.txt"
#define MAZEHEIGHT 20
#define MAZEWIDTH 20

typedef struct
{
    unsigned int top:2;
    unsigned int right:2;
    unsigned int bottom:2;
    unsigned int left:2;
    unsigned int name:8;
}mapNode;

mapNode map[ MAZEHEIGHT ][ MAZEWIDTH ];

int readMaze();
int drawMaze();
void test();

int main(void)
{
    int err = readMaze();
    err = (err<<1) + drawMaze();
    test();
    if (err)
    {
        printf("err code: %d\n", err);
        return 1;
    }
    else
    {
        return 0;
    }
}

int readMaze()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(MAZEFILE, "r");
    if (fp == NULL)
    {
        return 1;
    }

    while ((read = getline(&line, &len, fp)) != -1) 
    {
        const mapNode init = {.top = 0, .right = 0, .bottom = 0, .left = 0, .name = ' '};
        char *tok = strtok(line,"\t");
        int x,y;

        for(int i=0; tok != NULL; i++ )
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
                map[x][y].top = atoi(tok);
                break;
            case 3:
                map[x][y].right = atoi(tok);
                break;
            case 4:
                map[x][y].bottom = atoi(tok);
                break;
            case 5:
                map[x][y].left = atoi(tok);
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
    return 0;
}

int drawMaze()
{
    for(int y = MAZEHEIGHT -1; y >= 0; y--)
    {
        for(int x = 0; x<MAZEWIDTH; x++)
        {
            printf("+");
            if(map[x][y].top == 1)
            {
                printf("-----");
            }
            else if(map[x][y].top == 2)
            {
                printf("-] [-");
            }
            else
            {
                printf("     ");
            }
        }
        printf("+\n");
        for(int x = 0; x<MAZEWIDTH; x++)
        {
            if(map[x][y].left == 1)
            {
                printf("|  ");
                printf("%1c  ", map[x][y].name);
            }
            else if(map[x][y].left == 2)
            {
                printf("=  ");
                printf("%1c  ", map[x][y].name);
            }
            else
            {
                printf("   ");
                printf("%c  ", map[x][y].name);
            }
        }
        if(map[MAZEWIDTH - 1][y].right == 1)
        {
            printf("|\n");
        }
        else if(map[MAZEWIDTH -1][y].right == 2)
        {
            printf("=\n");
        }
    }

    for(int x = 0; x < MAZEWIDTH; x++)
    {
        if(map[x][0].bottom == 1)
        {
            printf("+-----");
        }
        else if(map[x][0].bottom == 2)
        {
            printf("+-] [-");
        }
        else
        {
            printf("+     ");
        }
    }
    printf("+\n");
    return 0;
}

void test()
{
    //
}