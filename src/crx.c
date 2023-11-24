

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "crx.h"
#include "comparator.h"
#include "debug.h"

// takes a File and runs the proper compile command for that language
// compilation errors are redirected to a default file to be reported to the user
CRXResult compile(File *file)
{
    char compilecommand[MAX_FILE_NAME_LEN + 30]; // reconsider position
    char* errorfile = "2> error.txt";         // the file where compilation errors are recorded

    switch (file->language)
    {
    case JAVA:
    {
        snprintf(compilecommand, sizeof(compilecommand), "javac %s.java %s", file->absolutefilepath, errorfile);
        int result = system(compilecommand);
        if (result == 0)
        {
            file->compiled = true;
            system("rm -f error.txt");
            return COMPILE_OK;
        }
        break;
    }
    case NONE:
        fprintf(stderr, "Unrecognized file type.\n");
    }
    return COMPILE_ERROR;
}

// returns true if c is a single or double quote
static bool is_string(const char c)
{
    return c  == '"' || c == '\'';
}

// return a new string that is a substring from start to end of the original
// the new string is always nul terminated
static char* substring(const char* string, int start, int end)
{
    char* result = (char*)malloc(end + 1);
    if (result == NULL)
    {
        fprintf(stderr, "Insoficient Memory.\n");
        return NULL;
    }
    for (int i = 0; i < end; i++)
    {
        result[i] = string[start + i];
    }
    result[end] = '\0';
    return result;
}

// returns the expected output by removing surrounding quotes and the leading space
static char* expected(Stream output)
{
    if (is_string(output.stream[1]))
    {
        return substring(output.stream, 2, output.length - 3); // removing leading space and surrounding quotes
    }
    return substring(output.stream, 1, output.length - 1); // removing leading space
}

// executes the program with each input of the io and stores the output and result status in the
// test results
CRXResult run_test(const File *file, const IO io, TestResults* testresults)
{
    char* outfile = "> temp.txt";
    char* errorfile = "2> error.txt";
    switch (file->language)
    {
        case JAVA:
            for (int i = 0; i < io.numtestcases; i++)
            {
                char runcommand[MAX_FILE_NAME_LEN + io.testcases[i].input.length + 20];
                snprintf(runcommand, sizeof(runcommand), "java %s %s %s %s", file->absolutefilepath, io.testcases[i].input.stream, outfile,  errorfile);
                #ifdef DEBUG_MODE
                    print_run_command(runcommand);
                #endif
                testresults->results[i].input = substring(io.testcases[i].input.stream, 1, io.testcases[i].input.length - 1);
                testresults->results[i].expected = expected(io.testcases[i].output);
                int result = system(runcommand);
                if (result == 0)
                {
                    system("rm -f error.txt");
                    testresults->results[i].received = read_file("temp.txt");
                    system("rm -f temp.txt");
                    if (testresults->results[i].received == NULL) {
                        testresults->results[i].received = "NULL";
                    }
                    testresults->results[i].status = compare(testresults->results[i].expected, testresults->results[i].received);
                }
                else
                {
                    testresults->results[i].received = read_file("error.txt");
                    testresults->results[i].status = FAIL;
                }
            }
            return RUN_OK;
        case NONE:
            fprintf(stderr, "Not an executable file.\n");
    }
    system("rm -f temp.txt");
    system("rm -f temp.txt");
    return RUN_ERROR;
}

// executes the program with each check input of the io and stores the output and result status in
// the test results
CRXResult run_check(const File *file, const IO io, TestResults* testresults)
{
    char *outfile = "> temp.txt";
    char *errorfile = "2> error.txt";
    int checkcount = 0;
    switch (file->language)
    {
    case JAVA:
        for (int i = 0; i < io.numtestcases; i++)
        {
            if (!(io.testcases[i].ischeckcase)) continue;
            char runcommand[MAX_FILE_NAME_LEN + io.testcases[i].input.length + 20];
            snprintf(runcommand, sizeof(runcommand), "java %s %s %s %s", file->absolutefilepath, io.testcases[i].input.stream, outfile, errorfile);
            #ifdef DEBUG_MODE
                print_run_command(runcommand);
            #endif
            testresults->results[checkcount].input = substring(io.testcases[i].input.stream, 1, io.testcases[i].input.length - 1);
            testresults->results[checkcount].expected = expected(io.testcases[i].output);
            int result = system(runcommand);
            if (result == 0)
            {
                system("rm -f error.txt");
                testresults->results[checkcount].received = read_file("temp.txt");
                system("rm -f temp.txt");
                if (testresults->results[checkcount].received == NULL)
                {
                    testresults->results[checkcount].received = "NULL";
                }
                testresults->results[checkcount].status = compare(testresults->results[checkcount].expected, testresults->results[checkcount].received);
            }
            else
            {
                testresults->results[checkcount].received = read_file("error.txt");
                testresults->results[checkcount].status = FAIL;
            }
            checkcount++;
        }
        return RUN_OK;
    case NONE:
        fprintf(stderr, "Not an executable file.\n");
    }
    system("rm -f temp.txt");
    system("rm -f temp.txt");
    return RUN_ERROR;
}

// entry to the run for both checks and submits
CRXResult run(const File *file, const IO io, TestResults*  testresults, RunType type)
{
    switch (type)
    {
        case CRX_TEST:
            return run_test(file, io, testresults);
        case CRX_CHECK:
            return run_check(file, io, testresults);
    }
    return RUN_ERROR;
}