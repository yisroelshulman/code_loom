#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"

#define MAX_LINE_WIDTH 80

// prompt to make the selection
static void prompt_selection()
{
    printf("Please select from the following options:\n");
    printf("[to make a selection please enter the number associated with the selection.]\n\n");
}

// dashed line separator
static void line_separator()
{
    for (int i = 0; i < MAX_LINE_WIDTH; i++)
    {
        printf("-");
    }
    printf("\n");
}

// the selection menu options that the user can choose from
static void show_selection_menu()
{
    char* menu = "Main Menu";
    printf("%*s%s\n", (int)(MAX_LINE_WIDTH - strlen(menu)) / 2, "", menu);
    line_separator();
    prompt_selection();
    printf("\t1. help\n");
    printf("\t2. default problem\n");
    printf("\t3. problem list\n");
    printf("\t4. exit\n");
}

// the run menu options that the user can choose from
static void show_run_menu()
{
    char* menu = "Run Menu";
    printf("%*s%s\n", (int)(MAX_LINE_WIDTH - strlen(menu)) / 2, "", menu);
    line_separator();
    prompt_selection();
    printf("\t1. check\n");
    printf("\t2. submit\n");
    printf("\t3. back to main menu\n");
}

// the submit prompt
static void show_filename_prompt()
{
    printf("Please enter the file name [path/filename]: \n");
}

// the submit menu
static void show_repeat_menu()
{
    prompt_selection();
    printf("\t1. try again\n");
    printf("\t2. back\n");
}

static void show_problem_list()
{
    char* menu = "Program List Menu";
    printf("%*s%s\n", (int)(MAX_LINE_WIDTH - strlen(menu)) / 2, "", menu);
    line_separator();
    prompt_selection();
    printf("\t1. palindrome\n");
    printf("\t2. sum ints from m to n\n");
    printf("\t3. back to main menu\n");
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
    char c[3];
    fgets(c, sizeof(c), stdin);
    if (c[1] == '\n') return c[0]; // exactly one character was read
    if (!(c[0] == '\n')) consume();
    return '0'; // 0 or too many characters were read
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

// extracts the run selection from the users input
static Selection get_run_selection()
{
    switch (to_int(read_selection()))
    {
        case 1:     return CHECK;
        case 2:     return SUBMIT;
        case 3:     return BACK;
        default:    return INVALID;
    }
    return INVALID; // unreachable
}

// extracts the submit selection from the users input
static Selection get_submit_selection()
{
    switch (to_int(read_selection()))
    {
        case 1:     return REPEAT;
        case 2:     return BACK;
        default:    return INVALID;
    }
    return INVALID; // unreachable
}

static Selection get_problem_list_selection()
{
    switch (to_int(read_selection()))
    {
    case 1:
        return PALINDROME;
    case 2:
        return SUM_INTS_M_TO_N;
    case 3:
        return BACK;
    default:
        return INVALID;
    }
    return INVALID; // unreachable
}

// ==================================================================================================================================================================
// remove the clear screen
// clears the screen in linux based distros
static void clear_screen()
{
    //system("clear");
}

// the selection menu and handling of the selection
Selection menu(Menu menu)
{
    switch (menu)
    {
        case SELECTION:
            clear_screen();
            show_selection_menu();
            return get_menu_selection();
        case RUN:
            clear_screen();
            show_run_menu();
            return get_run_selection();
        case FILE_NAME_PROMPT:
            clear_screen();
            show_filename_prompt();
            return INVALID;
        case REPEAT_MENU:
            clear_screen();
            show_repeat_menu();
            return get_submit_selection();
        case PROBLEM_LIST_MENU:
            show_problem_list();
            return get_problem_list_selection();
        default:
            return INVALID;
    }
    return INVALID;
}