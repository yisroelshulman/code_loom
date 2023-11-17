

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "crx.h"
#include "comparator.h"

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

static bool is_string(const char* expected)
{
    return expected[1] == '"';
}

static char* expected(char* expected)
{
    if (is_string(expected))
    {
        int length = 0;
        while (expected[length] != '\0') length++;
        char* result = (char*)malloc(length - 1);
        for (int i = 1; i < length - 1; i++)
        {
            result[i - 1] = expected[i];
        }
        result[0] = ' ';
        result[length - 1] = '\0';

        return result;
    }
    return expected;
}

CRXResult run(File *file, const IO io, TestResults* testresults)
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
                printf("%s\n", runcommand);
                testresults->results[i].input = io.testcases[i].input.stream;
                testresults->results[i].expected = expected(io.testcases[i].output.stream);
                int result = system(runcommand);
                if (result == 0)
                {
                    system("rm -f error.txt");
                    testresults->results[i].received = read_file("temp.txt");
                    if (testresults->results[i].received == NULL) {
                        testresults->results[i].received = "NULL";
                    }
                    testresults->results[i].status = compare(testresults->results[i].expected, testresults->results[i].received);
                }
            }
            print_result(testresults);
            return RUN_OK;
        case NONE:
            fprintf(stderr, "Not an executable file.\n");
    }
    system("rm -f temp.txt");
    return RUN_ERROR;
}