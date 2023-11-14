#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crx.h"
#include "file.h"
#include "io.h"
#include "translator.h"

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

static void consume()
{
    char c = getchar();
    while (c != '\n')
        c = getchar();
    return;
}

static void wait()
{
    char c;
    fgets(&c, 2, stdin);
    consume();
    printf("char = %c\n", c);
}

int main(int argc, char *argv[])
{
    bool flag = true;
    printf("%s\n", flag ? "true" : "false");

    switch (argc)
    {
        case 1:
            // error
            printf("error no args\n");
            exit(1);
        case 2:
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

                        char* source = read_file("test.sul");
                        translate(source, &io);
                        free(source);

                        run(&sourcecode, io);
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

                    char* source = read_file(argv[2]);
                    translate(source, &io);
                    free(source);

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