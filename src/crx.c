
#include <stdio.h>
#include <stdlib.h>

#include "crx.h"

// takes a File and runs the proper compile command for that language
// compilation errors are redirected to a default file to be reported to the user
CRXResult compile(File *file)
{
    char compilecommand[MAX_FILE_NAME_LEN + 30]; // reconsider position
    char *errorredirect = "2>error.txt";         // the file where compilation errors are recorded

    switch (file->language)
    {
    case JAVA:
    {
        snprintf(compilecommand, sizeof(compilecommand), "javac %s.java %s", file->absolutefilepath, errorredirect);
        int result = system(compilecommand);
        if (result == 0)
        {
            file->compiled = true;
            system("rm error.txt");
            return COMPILE_OK;
        }
        break;
    }
    case NONE:
        fprintf(stderr, "Unrecognized file type.\n");
    }
    return COMPILE_ERROR;
}

CRXResult run(File *file, const IO io)
{
    char* tempout = "> temp.txt";
    char* errorout = "2> error.txt";
    switch (file->language)
    {
        case JAVA:
            for (int i = 0; i < io.numtestcases; i++)
            {
                char runcommand[MAX_FILE_NAME_LEN + io.testcases[i].input.length + 20];
                snprintf(runcommand, sizeof(runcommand), "java %s %s %s %s", file->absolutefilepath, io.testcases[i].input.stream, tempout,  errorout);
                printf("%s\n", runcommand);
                int result = system(runcommand);
                if (result == 0)
                {
                    system("rm error.txt");
                    return RUN_OK;
                }
            }
        default:
            break;
    }
    system("rm temp.txt");
    return RUN_ERROR;
}