#ifndef AU_RESULT_H
#define AU_RESULT_H

#include <stdbool.h>

// the status resulting from testing
typedef enum
{
    PASS,
    FAIL
} Status;

// stores all the test information and result of the test
typedef struct
{
    Status status;
    char* input;
    char* expected;
    char* received;
} Result;

// stores the Results for all the testcases that were run
// the TestResult owns all the memory that was allocated to each Result in the results array
typedef struct
{
    int count;
    Result* results;
} TestResults;

/*
 * The init_result function initializes the TestResults for the test cases being run
 *
 * @param TestResults* testresults
 *      the pointer to the TestResults we are allocating memory to
 * @param const int count
 *      the number of tests being run
 * 
 * @returns
 *      true:   if the allocation worked
 *      false:  if the allocation failed
 */
bool init_result(TestResults* testresults, const int count);

/*
 * The free_result function frees all the memory that was allocated to the TestResults
 *
 * After the call to free_result the TestResults should no longer be used
 * 
 * @param TestResults* testresults
 *      the TestResult pointer that is being deallocated
 */
void free_result(TestResults* testresults);

/*
 * The output_result function prints the results for all the testcase that were run
 * on a passed test the printer will print the input and that it passed.
 * on a failed test the printer will print the input, expected output, and the received output.
 */
void output_result(const TestResults *testresults, char* file);

#endif