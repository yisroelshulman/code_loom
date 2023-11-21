#ifndef AG_IO_H
#define AG_IO_H

#include <stdbool.h>

#include "scanner.h"

// The Stream struct holds the input or expected output of the program
typedef struct
{
    int length;
    int capacity;
    char* stream;
} Stream;

// The TestCase struct holds the input and expected output of the testcases, and whether or not it
// should be run as a check or for a submit
typedef struct
{
    bool  ischeckcase;
    Stream input;
    Stream output;
} TestCase;

// The IO struct stores all the TestCases and how many are to be run
typedef struct
{
    int numtestcases;
    int capacity;
    TestCase* testcases;
} IO;

/*
 * The write_stream function takes a token and a stream and copies the token to the stream keeping
 * track of how many characters from the token were copied to the stream and returns it
 * 
 * @param Stream* stream
 *      the stream being written to
 * @param const Token token
 *      the token being written to the stream
 * 
 * @returns
 *      size_t >0: an integer that is the total characters added to the stream
 *      size_t 0: if the write was unsuccessful
 */
size_t write_stream(Stream* stream, const Token token);

/*
 * The init_test_case function initializes a test case to its default values.
 *
 * After a call to init_test_case the testcase will have the following values:
 * isCheckCase = false, input = {length = 0, capacity = 0, stream = NULL }, output = {length = 0,
 *      capacity = 0, stream = NULL }
 * 
 * @param TestCase* testcase
 *      the TestCase being initialized
 */
void init_test_case(TestCase* testcase);

/*
 * The free_test_case function initializes frees the allocated memory to a TestCase
 * 
 * After a call to free_test_case the testcase = NULL
 * 
 * @param TestCase* testcase
 *      the testcase who's memory is being deallocated
 */
void free_test_case(TestCase* testcase);

/*
 * The init_io function initializes a io to its default values
 * 
 * After a call to init_io the IO will have the following values:
 * numtestcases = 0, capacity = 0, testcases = NULL
 * 
 * @param IO* io
 *      the IO being initialized
 */
void init_io(IO* io);

/*
 * The free_io function frees the memory allocated to the IO
 * 
 * After a call to free_io the IO will have the values as after a call to init_io
 * 
 * @param IO* io
 *      the IO who's memory id being deallocated
 */
void free_io(IO* io);

/*
 * The add_test_case function adds a testcase to the IO and returns whether or not it was successful
 * 
 * @param IO* io
 *      the IO where the TestCase is added to
 * @param const TestCase* testcase
 *      the testcase being added to the IO
 * 
 * @returns
 *      true:   if the testcase was successfully added to the IO
 *      false:  if the testcase failed to be added to the IO
 */
bool add_test_case(IO *io, const TestCase* testcase);

#endif