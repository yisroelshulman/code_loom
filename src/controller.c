#include <stdio.h>
#include <stdlib.h>

#include "controller.h"
#include "crx.h"
#include "menu.h"
#include "file.h"
#include "io.h"
#include "result.h"
#include "translator.h"

IO io;

#define DEFAULT_FILE "input.sul"

static void consume()
{
    char c = getchar();
    while (c != '\n')
        c = getchar();
    return;
}

static void no_file(char source[MAX_FILE_NAME_LEN])
{
    source[0] = 'N';
    source[1] = '\\';
    source[2] = 'A';
    source[3] = '\0';}

static bool read_filename(char source[MAX_FILE_NAME_LEN], const int length)
{
    for (int i = 0; i <= length; i++)
    {
        char c = getchar();
        if (c == '\n')
        {
            source[i] = '\0';
            return true;
        }
        source[i] = c;
    }
    consume();
    no_file(source);
    return false;
}

static void submit()
{
    File sourcecode;
    while (1)
    {
        menu(SUBMIT_PROMPT);
        char source[MAX_FILE_NAME_LEN];
        if (!read_filename(source, MAX_FILE_NAME_LEN) || !init_file(&sourcecode, source))
        {
            printf("[%s] not a recognized compilable file.\n", source);
            if (menu(SUBMIT_MENU) == BACK) return;
            continue;
        }
        break;
    }
    compile(&sourcecode);

    TestResults testresults;
    init_result(&testresults, io.numtestcases);
    run(&sourcecode, io, &testresults);
    print_result(&testresults);
    menu(PAUSE);
}

// takes a string which contains the path to the source .sul file and translates it into test cases
// then extracts the users run option to run the program and test it
static void run_menu(char *sourcecode)
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
            printf("check\n");
            loop = 0;
            break;
        case SUBMIT:
            printf("submit\n");
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
            return;
        case DEFAULT_PROBLEM:
            run_menu(DEFAULT_FILE);
            break;
        case PROBLEM_LIST:
            printf("problem list\n");
            return;
        case EXIT:
            printf("exit\n");
            return;
        default:
            printf("Invalid Selection\n");
        }
    }
}