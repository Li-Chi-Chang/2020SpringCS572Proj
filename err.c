#include "err.h"

int isInitErrLog = 0;

int err(int code)
{
    switch (code)
    {
    case 0:
        return 0;
    case FILEERR:
        printf("Error here.\nerr code: %d FILE ERR!\n",code);
        exit(1);
    case NOINITERR:
        printf("Error here.\nerr code: %d NO INIT ERR!\n",code);
        exit(1);
    case COMMANDNOTFOUND:
        printf("Error here.\nerr code: %d COMMAND NOT FOUND!\n",code);
        exit(1);
    case BREAKPOINT:
        printf("Break Point\n");
        exit(1);
    default:
        return code;
    }
}

int logFile(char* string)
{
    if(!isInitErrLog)
    {
        return err(NOINITERR);
    }
    char command[MAXSTRINGSIZE] = "";
    strcat(command, "echo \"");
    strcat(command, string);
    strcat(command, "\"  >> log.txt\n");
    system(command);

    return 0;
}

void initErrLog()
{
    isInitErrLog = 1;
    system("rm log.txt\n");
}