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

void free_rslt(Result* result)
{
    free(result->input);
    free(result->expected);
    free(result->received);
}

void free_result(TestResults* testresults)
{
    for (int i = 0; i < testresults->count; i++)
    {
        free_rslt(&testresults->results[i]);
    }
    free(testresults->results);
    testresults->count = 0;
}

static void print_case_result(Result result)
{
    printf("On input: %s\n", result.input);
    switch (result.status)
    { 
        case PASS:
            printf("result: pass\n");
            return;
        case FAIL:
            printf("Expected: ");
            int i = 0;
            while (result.expected[i] != '\0')
            {
                printf("%c", result.expected[i]);
                i++;
            }
            printf("\nReceived: %s\n", result.received);
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