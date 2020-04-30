#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRINGSIZE 1024

int err(int);
int logFile(char*);
void initErrLog();

char LogFileString[MAXSTRINGSIZE];

// error codes
#define NOINITERR 0x01
#define FILEERR 0x02
#define COMMANDNOTFOUND 0x03
#define BREAKPOINT 0x04
#define SOMETHINGEMPTY 0x05