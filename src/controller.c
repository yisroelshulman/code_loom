#include <stdio.h>
#include <stdlib.h>

#include "controller.h"
#include "crx.h"
#include "debug.h"
#include "menu.h"
#include "file.h"
#include "io.h"
#include "result.h"
#include "translator.h"

IO io;
TestResults testresults;

#define DEFAULT_FILE "input.sul"

// consumes the characters in stdin until a newline character is read
static void consume()
{
    char c = getchar();
    while (c != '\n')
        c = getchar();
    return;
}

// sets the char* at source to N/A
static void no_file(char source[MAX_FILE_NAME_LEN])
{
    source[0] = 'N';
    source[1] = '\\';
    source[2] = 'A';
    source[3] = '\0';}

// reads up to length characters into source. if no newline character was read in length characters
// source will be set to N/A. This function also clears the stdin buffer
static bool read_filename(char source[MAX_FILE_NAME_LEN], const int length)
{
    for (int i = 0; i <= length; i++)
    {
        char c = getchar();
        if (c == '\n') // newline character
        {
            source[i] = '\0';
            return true;
        }
        source[i] = c;
    }
    consume(); // clear the buffer
    no_file(source); // set source to N/A
    return false;
}

// submit asks the user the filename for the file containing the program being tested.
// and the compiles, and tests the program and reports the result to the user.
static void submit()
{
    File sourcecode;
    while (1)
    {
        menu(FILE_NAME_PROMPT);
        char source[MAX_FILE_NAME_LEN];
        if (!read_filename(source, MAX_FILE_NAME_LEN) || !init_file(&sourcecode, source))
        {
            printf("[%s] not a recognized compilable file.\n", source);
            if (menu(REPEAT_MENU) == BACK) return;
            continue;
        }
        break;
    }
    compile(&sourcecode);
    init_result(&testresults, io.numtestcases);
    run(&sourcecode, io, &testresults, CRX_TEST);
    output_result(&testresults, "result.txt");
    free_result(&testresults);
    system("less result.txt");
    // log?
    system("rm -f result.txt");
}

static void check()
{
    File sourcecode;
    while (1)
    {
        menu(FILE_NAME_PROMPT);
        char source[MAX_FILE_NAME_LEN];
        if (!read_filename(source, MAX_FILE_NAME_LEN) || !init_file(&sourcecode, source))
        {
            printf("[%s] not a recognized compilable file.\n", source);
            if (menu(REPEAT_MENU) == BACK)
                return;
            continue;
        }
        break;
    }
    compile(&sourcecode);
    init_result(&testresults, io.numcheckcases);
    run(&sourcecode, io, &testresults, CRX_CHECK);
    printf("returned\n");
    output_result(&testresults, "result.txt");
    free_result(&testresults);
    system("less result.txt");
    // log?
    system("rm -f result.txt");
}

// =====================================================================================================================================================================================
// takes a string which contains the path to the source .sul file and translates it into test cases
// then extracts the users run option to run the program and test it
static void run_controller(char *sourcecode)
{
    init_io(&io);
    char *source = read_file(sourcecode);
    translate(source, &io);
    free(source);

    int loop = 1;
    while (loop)
    {
        switch (menu(RUN))
        {
            case CHECK:
                check();
                loop = 0;
                break;
            case SUBMIT:
                submit();
                loop = 0;
                break;
            case ADD_TEST_CASE:
                printf("add test case\n");
                loop = 0;
                break;
            case BACK:
                printf("back\n");
                loop = 0;
                break;
            default:
                printf("Invalid selection\n");
        }
    }
    free_io(&io);
    return;
}

static void problem_list()
{
    while (1)
    {
        switch (menu(PROBLEM_LIST_MENU))
        {
            case PALINDROME:
                printf("palindrome.\n");
                break;
            case SUM_INTS_M_TO_N:
                printf("sum ints m-n\n");
                break;
            case BACK:
                printf("back.\n");
                return;
            default:
                printf("Invalid selection.\n");
        }
    }
}


// ===============================================================================================================================================================================================
// entry into the program when 0 args were passed
// brings up the selection menu
void start()
{
    while (1)
    {
        switch (menu(SELECTION))
        {
            case HELP:
                printf("help\n");
                break;
            case DEFAULT_PROBLEM:
                run_controller(DEFAULT_FILE);
                break;
            case PROBLEM_LIST:
                problem_list();
                break;
            case EXIT:
                printf("exit\n");
                return;
            default:
                printf("Invalid Selection\n");
        }
    }
}