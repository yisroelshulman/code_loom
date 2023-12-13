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

#define NUM_PROGRAMS 2
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

static void run_check(char *sourcecode)
{
    init_io(&io);
    char *source = read_file(sourcecode);
    translate(source, &io);
    free(source);

    check();

    free_io(&io);
    return;
}

static void run_submit(char *sourcecode)
{
    init_io(&io);
    char *source = read_file(sourcecode);
    translate(source, &io);
    free(source);

    submit();

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
                system("less help_files/help_file.info");
                break;
            case DEFAULT_PROBLEM:
                run_controller(SUL_DEFAULT_FILE);
                break;
            case PROBLEM_LIST:
                problem_list();
                break;
            case EXIT:
                printf("exit\n");
                return 0;
            default:
                printf("Invalid Selection\n");
        }
    }
    return 0;
}

static bool match(char *flag, const int offset, const int len, const char *rest)
{
    return memcmp(flag + offset, rest, len) == 0;
}

typedef enum
{
    OP_CHECK,
    OP_DEFAULT,
    OP_HELP,
    OP_LIST,
    OP_PROGRAM,
    OP_SUBMIT,
    OP_ERROR
} OP;

static OP match_op(char *option, const int len, const char *rest, const OP type)
{
    if (match(option, 1, len, rest)) return type;
    return OP_ERROR;
}

static OP get_op(char *option)
{
    switch(option[0])
    {
        case 'c': return match_op(option, 5, "heck", OP_CHECK);
        case 'd': return match_op(option, 7, "efault", OP_DEFAULT);
        case 'l': return match_op(option, 4, "ist", OP_LIST);
        case 'p': return match_op(option, 7, "rogram", OP_PROGRAM);
        case 's': return match_op(option, 6, "ubmit", OP_SUBMIT);
    }
    return OP_ERROR;
}

int start_one(char *flag)
{
    switch (get_op(flag))
    {
        case OP_CHECK:
            run_check(SUL_DEFAULT_FILE);
            return 0;
        case OP_DEFAULT:
            run_controller(SUL_DEFAULT_FILE);
            return 0;
        case OP_LIST:
            problem_list();
            return 0;
        case OP_SUBMIT:
            run_submit(SUL_DEFAULT_FILE);
            return 0;
        default:
            switch (flag[0])
            {
                case '-':
                    if (match(flag, 1, 2, "h") || match(flag, 1, 5, "help"))
                    {
                        system("less help_files/help_file.info");
                        return 0;
                    }
                    if (match(flag, 1, 3, "hc"))
                    {
                        system("less -p -hc help_files/help_file.info");
                        return 0;
                    }
                    if (match(flag, 1, 3, "hd"))
                    {
                        system("less -p -hd help_files/help_file.info");
                        return 0;
                    }
                    if (match(flag, 1, 3, "hl"))
                    {
                        system("less -p -hl help_files/help_file.info");
                        return 0;
                    }
                    if (match(flag, 1, 3, "hp"))
                    {
                        system("less -p -hp help_files/help_file.info");
                        return 0;
                    }
                    if (match(flag, 1, 3, "hs"))
                    {
                        system("less -p -hs help_files/help_file.info");
                        return 0;
                    }
                default:
                    return 56663;
            }
    }
    return 56663;
}

static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static int my_atoi(char *str)
{
    int sum = 0;
    while (*str)
    {
        if (is_digit(str[0]))
        {
            sum *= 10;
            sum += str[0] - '0';
        }
        else
        {
            return -1;
        }
        str++;
    }
    if (sum > NUM_PROGRAMS) return -2;
    return sum;
}

int start_two(char *flag, char *option)
{
    printf("%s\n", flag);
    if (match(flag, 0, 3, "-h") || match(flag, 0, 6, "-help")) // len + 1 for nul terminator
    {
        switch (get_op(option))
        {
            case OP_CHECK:
                system("less -p \"-h check\" help_files/help_file.info");
                return 0;
            case OP_DEFAULT:
                system("less -p \"-h default\" help_files/help_file.info");
                return 0;
            case OP_LIST:
                system("less -p \"-h list\" help_files/help_file.info");
                return 0;
            case OP_PROGRAM:
                system("less -p \"-h program\" help_files/help_file.info");
                return 0;
            case OP_SUBMIT:
                system("less -p \"-h submit\" help_files/help_file.info");
                return 0;
            default:
                 return 56663;
        }
    }
    else if (match(flag, 0, 8, "program"))
    {
        switch (my_atoi(option))
        {
            case 1:
                system(CMD_PALINDROME_INSTRUCTIONS);
                run_controller(SUL_PALINDROME_FILE);
                return 0;
            case 2:
                system(CMD_SUM_INTS_M_TO_N_INSTRUCTIONS);
                run_controller(SUL_SUM_INTS_M_TO_N);
                return 0;
            default:
                printf("error\n");
                return 56663;
        }
    }

    return 56663;
}