#include <stdio.h>
#include <stdlib.h>

#include "controller.h"

int main(int argc, char *argv[])
{
    switch (argc)
    {
        case 1:
            start_zero();
            exit(0);
        case 2:
            start_one(argv[2]);
            exit(2);
        case 3:
            start_two(argv[2], argv[3]);
            exit(3);
        default:
            fprintf(stderr, "[%d] invalid number of args.\n", argc);
            exit(1);
    }
}