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
    bool  ischeckcase;
    Stream* input;
    Stream* output;
} TestCase;

typedef struct
{
    TestCase* testcases;
    int numtestcases;
    int capacity;
} IO;

void init_io(IO* io);
void free_io(IO* io);
void add_test_case(TestCase* testcase);

#endif