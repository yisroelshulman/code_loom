#include <stdbool.h>

#ifndef IO_H
#define IO_H

typedef struct
{
    char* stream;
    int length;
    int capacity;
} Stream;

typedef struct
{
    bool  istestcase;
    Stream* input;
    Stream* output;
} Case;

typedef struct
{
    Case* cases;
    int numcases;
    int capacity;
} IO;

void init_io(IO* io);
void free_io(IO* io);
void add_input(IO* io);
void add_output(IO* io);

#endif