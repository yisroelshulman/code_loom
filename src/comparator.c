#include "comparator.h"

// the struct containing the relevant information for the comparator
typedef struct
{
    const char* expected;
    const char* received;
    int posexpected;
    int posreceived;
} Comparator;

// the comparator that is used
Comparator comparator;

// the  whitespaces that might be found in the outputs
static bool is_whitespace(char c)
{
    return c == ' ' || c == '\r' || c == '\t' || c == '\n';
}

// moves to the next comparable character in the expected string
static void advance_expected()
{
    comparator.expected++;
    while (is_whitespace(comparator.expected[comparator.posexpected])) comparator.expected++;
}

// moves to the next comparable character in the received string
static void advance_received()
{
    comparator.received++;
    while (is_whitespace(comparator.received[comparator.posreceived])) comparator.received++;
}

// advances to the next comparable character in both the expected and received strings
static void advance()
{
    advance_expected();
    advance_received();
}

// compares the expected string with the received string and returns whether or not they are
// considered a match
Status compare(const char* expected, const char* received)
{
    comparator.expected = expected;
    comparator.received = received;
    comparator.posexpected = comparator.posreceived = -1; // starts at -1 to ignore leading whitespaces

    advance();

    while (comparator.expected[comparator.posexpected] == comparator.received[comparator.posreceived])
    {
        if (comparator.expected[comparator.posexpected] == '\0') return PASS;
        advance();
    }
    return FAIL;
}