
#include <stdio.h>

#include "menu.h"

#define MENU_SELECTIONS 4

typedef enum
{
    HELP,
    DEFAULT_PROBLEM,
    PROBLEM_LIST,
    EXIT,
    CHECK,
    SUBMIT,



    INVALID
} Selection;

// the menu options that the user can choose from
static void show_menu()
{
    printf("Please select from the following options:\n\n");
    printf("\t1. help\n");
    printf("\t2. default problem\n");
    printf("\t3. problem list\n");
    printf("\t4. exit\n");
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

// the selection menu and handling of the selection
void menu()
{
    while(1)
    {
        show_menu();
        switch (get_menu_selection())
        {
            case HELP:
                printf("help\n");
                return;
            case DEFAULT_PROBLEM:
                printf("default problem\n");
                return;
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