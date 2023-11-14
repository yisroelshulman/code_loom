#include <stdio.h>
#include <stdlib.h>

#include "result.h"

bool init_result(TestResults* testresults, int count)
{
    testresults->results = malloc(sizeof(Result) * count);
    if (testresults->results == NULL)
    {
        fprintf(stderr, "Not enough memory for the results.\n");
    }
    testresults->count = count;
}