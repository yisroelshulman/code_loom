
#include <stdio.h>
#include "comparator.h"

typedef struct
{
    char* expected;
    char* received;
    int posexpected;
    int posreceived;
} Comparator;

Comparator comparator;

static bool is_whitespace(char c)
{
    return c == ' ' || c == '\r' || c == '\t' || c == '\n';// || c == '"' || c == '\'';
}

static void advance_expected()
{
    comparator.expected++;
    while (is_whitespace(comparator.expected[comparator.posexpected])) comparator.expected++;
}

static void advance_received()
{
    comparator.received++;
    while (is_whitespace(comparator.received[comparator.posreceived])) comparator.received++;
}

static void advance()
{
    advance_expected();
    advance_received();
}



Status compare(char* expected, char* received)
{
    comparator.expected = expected;
    comparator.received = received;
    comparator.posexpected = comparator.posreceived = -1;

    advance();

    while (comparator.expected[comparator.posexpected] == comparator.received[comparator.posreceived])
    {
        if (comparator.expected[comparator.posexpected] == '\0') return PASS;
        advance();
    }
    return FAIL;
}