#ifndef AU_RESULT_H
#define AU_RESULT_H

#include <stdbool.h>

typedef enum
{
    PASS,
    FAIL
} Status;

typedef struct
{
    Status status;
    char* input;
    char* expected;
    char* received;
} Result;

typedef struct
{
    int count;
    Result* results;
} TestResults;

bool init_result(TestResults* testresults, int count);

#endif