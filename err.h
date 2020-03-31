#include <stdio.h>
#include <stdlib.h>

int err(int);

// error codes
#define NOINITERR 0x01
#define FILEERR 0x02
#define COMMANDNOTFOUND 0x03


#define CHECKPOINT system("echo \"hi1\" >> log");