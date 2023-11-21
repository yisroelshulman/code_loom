#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "io.h"
#include "menu.h"
#include "translator.h"

#define DEFAULT_FILE "input.sul"

typedef enum
{
    HELP,
    DEFAULT_PROBLEM,
    PROBLEM_LIST,
    EXIT,
    CHECK,
    SUBMIT,
    ADD_TEST_CASE,
    BACK,
    INVALID
} Selection;

IO io;

// the selection menu options that the user can choose from
static void show_selection_menu()
{
    printf("Please select from the following options:\n\n");
    printf("\t1. help\n");
    printf("\t2. default problem\n");
    printf("\t3. problem list\n");
    printf("\t4. exit\n");
    printf("\nto make a selection please enter the number associated with the selection.\n");
}

// the run menu options that the user can choose from
static void show_run_menu()
{
    printf("Please select from the following options:\n\n");
    printf("\t1. check\n");
    printf("\t2. submit\n");
    printf("\t3. add test case\n");
    printf("\t4. back\n");
    printf("\nto make a selection please enter the number associated with the selection.\n");
}

// consumes the rest of stdin until the newline character
static void consume()
{
    char c = getchar();
    while (c != '\n')
        c = getchar();
    return;
}

// reads a single character from stdin consumes until the newline character and returns the first
// character even if its the newline character
static char read_selection()
{
    char c[2];
    fgets(c, sizeof(c), stdin);
    if (!(c[0] == '\n')) consume();
    return c[0];
}

// converts an int stored as a char to an int
// the caller is responsible for validation
static int to_int(char c)
{
    return c - '0';
}

// extract the menu selection from the user input
static Selection get_menu_selection()
{
    switch (to_int(read_selection()))
    {
        case 1:     return HELP;
        case 2:     return DEFAULT_PROBLEM;
        case 3:     return PROBLEM_LIST;
        case 4:     return EXIT;
        default:    return INVALID;
    }
    return INVALID; // unreachable
}

// extracts the run selection from the user input
static Selection get_run_selection()
{
    switch (to_int(read_selection()))
    {
        case 1:     return CHECK;
        case 2:     return SUBMIT;
        case 3:     return ADD_TEST_CASE;
        case 4:     return BACK;
        default:    return INVALID;
    }
    return INVALID; // unreachable
}

// takes a string which contains the path to the source .sul file and translates it into test cases
// then extracts the users run option to run the program and test it
static void run_menu(char* sourcecode)
{
    init_io(&io);
    char* source = read_file(sourcecode);
    translate(source, &io);
    free(source);

    int loop = 1;
    while (loop)
    {
        show_run_menu();
        switch (get_run_selection())
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

// the selection menu and handling of the selection
void menu()
{
    while (1)
    {
        show_selection_menu();
        switch (get_menu_selection())
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