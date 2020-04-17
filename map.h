//printf file functions
#include <stdio.h>
#include <stdlib.h>
//atoi function
#include <string.h>
//sleep function
#include <unistd.h>
//usleep
//#include <time.h>

#include "err.h"
typedef struct
{
    unsigned int north:2;
    unsigned int east:2;
    unsigned int south:2;
    unsigned int west:2;
    unsigned int name:8;
    unsigned int reserve:16;
}mapNode;

// every program need to call this or the map is empty
int readMaze();
int goNext();
int rotate(int);
mapNode getNodeInfo();

// the information about Maze
#define MAZEFILE "mapWithName.txt"
#define MAZEHEIGHT 20
#define MAZEWIDTH 20

// the information about grid
#define WALL 1
#define GRID 0
#define ENTRY 2

// the information about mouse compass
#define NORTH 0
#define SOUTH 2
#define EAST 1
#define WEST 3
#define TOP 4
#define BOTTOM 6
#define RIGHT 5
#define LEFT 7
#define TRACE '#'

// the information about entry point
#define entryX 0
#define entryY 0
#define entryHead NORTH

// print the coordinate or not
#define ISPRINTCORD 1

// print the trace route or not
#define ISPRINTTRACE 0

// print name node or not
#define ISPRINTNAME 0

// print fork node or not
#define ISPRINTFORKNODE 1

// delaying time
//#define DELAYING 50000
#define DELAYING 200000
#define WITHOUTDELAYING 0