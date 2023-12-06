#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crx.h"
#include "file.h"
#include "io.h"
#include "translator.h"
#include "result.h"
#include "controller.h"

typedef enum
{
    SUBMIT,
    CHECK,
    HELP,
    TEST,
    ERROR
} OP;

OP get_op(char* op);
OP match_op(char* op, int len, const char *rest, OP type);

int main(int argc, char *argv[])
{
    bool flag = true;
    printf("%s\n", flag ? "true" : "false");
    printf("flag = %d\n", flag);

    switch (argc)
    {
        case 1:
            start();
            exit(0);
        case 2:
            // start();
            exit(1);
        case 3:
            switch (get_op(argv[1]))
            {
                case SUBMIT:
                {
                    File sourcecode;

                    if (init_file(&sourcecode, argv[2]))
                    {
                        print_file(&sourcecode);
                        printf("compilable\n");
                        compile(&sourcecode);
                        IO io;
                        init_io(&io);

                        char *source = read_file("program_files/fizz_buzz.sul");
                        translate(source, &io);
                        free(source);

                        TestResults testresults;
                        init_result(&testresults, io.numtestcases);


                        run(&sourcecode, io, &testresults, CRX_TEST);

                        output_result(&testresults, "result.txt");
                        free_result(&testresults);
                        free_io(&io);
                        system("less result.txt");
                        // log?
                        system("rm -f result.txt");
                    }
                    else
                    {
                        print_file(&sourcecode);
                        printf("[%s] can't compile.\n", argv[2]);
                    }
                    printf("submit\n");
                    break;
                }
                case CHECK:
                        printf("check\n");
                        break;
                case HELP:
                        printf("help\n");
                        break;
                case TEST:
                {
                    IO io;
                    init_io(&io);
                    char *source = read_file(argv[2]);
                    translate(source, &io);
                    free(source);
                    free_io(&io);
                    break;
                }
                default:
                        printf("error\n");
                        break;
            }
            break;
        default:
            switch (get_op(argv[1]))
            {
                case HELP:
                    printf("help\n");
                    break;
                default:
                    printf("error\n");
                    break;
            }
            break;
    }
    exit(0);

}

OP match_op(char* op, int len, const char* rest, OP type)
{
    if (memcmp(op + 1, rest, len) == 0) return type;

    return ERROR;
}

OP get_op(char* op)
{
    switch(op[0])
    {
        case 's':   return match_op(op, 6, "ubmit\0", SUBMIT);
        case 'c':   return match_op(op, 5, "heck\0", CHECK);
        case 'h':   return match_op(op, 4, "elp\0", HELP);
        case 't':   return match_op(op, 4, "est\0", TEST);
    }

    return ERROR;
}