#include <stdio.h>
#include <stdlib.h>

#include "controller.h"
#include "menu.h"
#include "file.h"
#include "io.h"
#include "translator.h"

IO io;

#define DEFAULT_FILE "input.sul"

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