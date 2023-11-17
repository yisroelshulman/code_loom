
#include <stdio.h>

#include "comparator.h"

Status compare(const char* expected, const char* received)
{
    int i = 0;
    // i + 1 is because the expected always starts with a leading space
    while ((expected[i + 1] != '\0') || (received[i] != '\0'))
    {
        if (expected[i + 1] != received[i]) return FAIL;
        i++;
    }
    return PASS;
}