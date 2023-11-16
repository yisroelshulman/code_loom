#include <stdio.h>
#include <stdlib.h>

#include "result.h"

bool init_result(TestResults* testresults, const int count)
{
    testresults->results = malloc(sizeof(Result) * count);
    if (testresults->results == NULL)
    {
        fprintf(stderr, "Not enough memory for the results.\n");
    }
    testresults->count = count;
}

static void print_case_result(Result result)
{

}

void print_result(const TestResults* testresults)
{
    printf("Results:\n");
    for (int i = 0; i < testresults->count; i++)
    {
        print_case_result(testresults->results[i]);
    }
}