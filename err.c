#include "err.h"

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
    case STACKEMPTY:
        printf("Error here.\nerr code: %d STACK EMPTY!\n",code);
        exit(1);
    case ALLOCATIONERR:
        printf("Error here.\nerr code: %d ALLOCATION ERR!\n",code);
        exit(1);
    default:
        return code;
    }
}