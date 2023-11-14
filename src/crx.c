
#include <stdio.h>
#include <stdlib.h>

#include "crx.h"

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

CRXResult run(File *file, const IO io)
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
                int result = system(runcommand);
                if (result == 0)
                {
                    system("rm -f error.txt");
                    char* result = read_file("temp.txt");
                    if (result == NULL) {
                        printf("nothing\n");
                    }
                    else 
                    {
                        printf("\nout start:\n%s\n", result);
                        printf("out end:\n");
                    }

                }
            }
            return RUN_OK;
        case NONE:
            fprintf(stderr, "Not an executable file.\n");
    }
    system("rm -f temp.txt");
    return RUN_ERROR;
}