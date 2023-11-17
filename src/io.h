#ifndef AG_IO_H
#define AG_IO_H

#include <stdbool.h>

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


size_t write_stream(Stream* stream, const Token token);
void init_test_case(TestCase* testcase);
void init_io(IO* io);
void free_io(IO* io);
bool add_test_case(IO *io, const TestCase *testcase);


#endif