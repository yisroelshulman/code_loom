#include <stdio.h>
#include <stdlib.h>

#include "result.h"

// initializes the testresults and returns true or false depending on if it worked
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

// frees the allocated memory for the Result
static void free_rslt(Result* result)
{
    free(result->input);
    free(result->expected);
    free(result->received);
}

// frees the allocated memory for the TestResults
void free_result(TestResults* testresults)
{
    for (int i = 0; i < testresults->count; i++)
    {
        free_rslt(&testresults->results[i]);
    }
    free(testresults->results);
    testresults->count = 0;
}

// prints to stdout the result from the testcase. with specified messages depending on if the test
// passed or failed
static void print_case_result(Result result)
{
    printf("On input: %s\n", result.input);
    switch (result.status)
    { 
        case PASS:
            printf("result: pass\n\n");
            return;
        case FAIL:
            printf("result: fail\n");
            printf("Expected: %s\n", result.expected);
            printf("Received: %s\n\n", result.received);
            return;
    }
}

// prints the results of the tests
void print_result(const TestResults* testresults)
{
    printf("Results:\n");
    for (int i = 0; i < testresults->count; i++)
    {
        print_case_result(testresults->results[i]);
    }
}