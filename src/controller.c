#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define SUL_DEFAULT_FILE "input.sul"
#define SUL_PALINDROME_FILE "program_files/palindrome.sul"
#define SUL_SUM_INTS_M_TO_N "program_files/sum_ints_m_to_n.sul"
#define CMD_PALINDROME_INSTRUCTIONS "less instruction_files/palindrome.info"
#define CMD_SUM_INTS_M_TO_N_INSTRUCTIONS "less instruction_files/sum_ints_m_to_n.info"

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
                system(CMD_PALINDROME_INSTRUCTIONS);
                run_controller(SUL_PALINDROME_FILE);
                return;
            case SUM_INTS_M_TO_N:
                system(CMD_SUM_INTS_M_TO_N_INSTRUCTIONS);
                run_controller(SUL_SUM_INTS_M_TO_N);
                return;
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
int start_zero()
{
    while (1)
    {
        switch (menu(SELECTION))
        {
            case HELP:
                printf("help\n");
                break;
            case DEFAULT_PROBLEM:
                run_controller(SUL_DEFAULT_FILE);
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
    return 0;
}

static bool match(char *flag, const int offset, const int len, char *rest)
{
    return memcmp(flag + offset, rest, len) == 0;
}

typedef enum
{
    CHECK,
    DEFAULT,
    LIST,
    PROGRAM,
    SUBMIT,
    ERROR
} OP;

static OP get_op(char *option)
{
    switch(option[0])
    {
        case 'c': return match_op(option, 5, "heck", CHECK);
        case 'd': return match_op(option, 7, "efault", DEFAULT);
        case 'l': return match_op(option, 4, "ist", LIST);
        case 'p': return match_op(option, 7, "rogram", PROGRAM);
        case 's': return match_op(option, 6, "ubmit", SUBMIT);
    }
    return ERROR;
}

int start_one(char *flag)
{
    return 56663;
}


int start_two(char *flag, char *option)
{
    printf("%s\n", flag);
    if (match(flag, 0, 3, "-h") || match(flag, 0, 6, "-help")) // len + 1 for nul terminator
    {
        switch (get_op(option))
        {
            case CHECK:
            case DEFAULT:
            case LIST:
            case PROGRAM:
            case SUBMIT:
            case ERROR:
        }
    }
    else if (match(flag, 0, 2, "p") || match(flag, 0, 8, "program"))
    {

    }
    else
        fprintf(stderr, "bad\n");

    return 56663;
}