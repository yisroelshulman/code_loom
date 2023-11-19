#ifndef AG_CRX_H
#define AG_CRX_H

#include "file.h"
#include "io.h"
#include "result.h"

typedef enum
{
    COMPILE_OK,
    COMPILE_ERROR,
    RUN_OK,
    RUN_ERROR
} CRXResult;

/*
 * The function compile_file compiles the source code so it can then be run
 * Any messages and stack traces the compiler reports will be logged in a local file "error.txt"
 *
 * @param File* file
 *      the file struct that contains the meta data so the source code can be compiled
 *
 * @returns
 *      COMPILE_OK:     if the compilation was successful
 *      COMPILE_ERROR:  if the compilation failed
 */
CRXResult compile(File* file);

/*
 * The run function executes the program with each test case from the io and compares the output to
 * the expected outut then stores the ouput and comparison result into the TestResults
 * 
 * After a call to run the initial File and IO will remain unchanged
 * 
 * @param const File* file
 *      the File containing the information needed to execute the program
 * @param const IO* io
 *      the IO that that contains all the test cases
 * @param TestResults* testresults
 *      the TestResult where the output and comparison result will be stored
 * 
 * @returns
 *      RUN_OK:     if the program is of a recognized file type
 *      RUN_ERROR:  if the program is not a recognized file type
 */
CRXResult run(const File* file, const IO io, TestResults* testresults);

#endif