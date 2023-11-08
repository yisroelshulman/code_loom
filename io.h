#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stddef.h>

#include "scanner.h"

typedef struct
{
    int length;
    int capacity;
    char* stream;
} Stream;

typedef struct
{
    bool  ischeckcase;
    Stream input;
    Stream output;
} TestCase;

typedef struct
{
    TestCase* testcases;
    int numtestcases;
    int capacity;
} IO;

void init_stream(Stream* stream);
size_t write_stream(Stream* stream, const Token token);


void init_io(IO* io);
void free_io(IO* io);
bool add_test_case(IO *io, const TestCase *testcase);


#endif