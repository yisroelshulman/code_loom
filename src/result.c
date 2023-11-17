#include <stdio.h>
#include <stdlib.h>

#include "result.h"

bool init_result(TestResults* testresults, const int count)
{
    testresults->results = malloc(sizeof(Result) * count);
    if (testresults->results == NULL)
    {
        fprintf(stderr, "Not enough memory for the results.\n");
        testresults->count = 0;
        return false;
    }
    testresults->count = count;
    return true;
}

static void print_case_result(Result result)
{
    printf("On input: %s\n", result.input);
    switch (result.status)
    { 
        case PASS:
            printf("result: pass");
            return;
        case FAIL:
            printf("Expected: %s\n", result.expected);
            printf("Received: %s\n", result.received);
            return;
    }
}

void print_result(const TestResults* testresults)
{
    printf("Results:\n");
    for (int i = 0; i < testresults->count; i++)
    {
        print_case_result(testresults->results[i]);
    }
}