#include <stdio.h>
#include <stdlib.h>

#include "controller.h"

int main(int argc, char *argv[])
{
    int exitcode = -1;
    switch (argc)
    {
        case 1:
            exitcode = start_zero();
            break;
        case 2:
            exitcode = start_one(argv[1]);
            break;
        case 3:
            exitcode = start_two(argv[1], argv[2]);
            break;
        default:
            fprintf(stderr, "[%d] invalid number of args.\n", argc);
            exitcode = 1;
    }
    if (exitcode != 0) fprintf(stderr, "try running with -h or -help for help using this tool\n");
    exit(exitcode);
}