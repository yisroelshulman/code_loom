#include <stddef.h>
#include <stdio.h>

#include "debug.h"
#include "scanner.h"

void print_input(Stream* stream)
{
    printf("input: %s\n", stream->stream);
}

void print_output(Stream* stream)
{
    printf("output: %s\n", stream->stream);
}

void print_case(TestCase* testcase)
{
    printf("is test case? %s\n", testcase->ischeckcase ? "true" : "false");
    print_input(&testcase->input);
    print_output(&testcase->output);
}

void print_io(IO* io)
{
    printf("cap = %d, count = %d\n\n", io->capacity, io->numtestcases);
    for (int i = 0; i < io->numtestcases; i++)
    {
        printf("test case # %d:\n------------------\n", i + 1);
        print_case(&io->testcases[i]);
        printf("\n");
    }
}

char *tokenasstring[] = {
    // symbols
    [TOKEN_LEFT_PARENT] = "left parent",
    [TOKEN_RIGHT_PARENT] = "right parent",
    [TOKEN_LEFT_BRACE] = "left brace",
    [TOKEN_RIGHT_BRACE] = "right brace",
    [TOKEN_LEFT_BRACKET] = "left bracket",
    [TOKEN_RIGHT_BRACKET] = "right bracket",
    [TOKEN_COLON] = "colon",
    [TOKEN_DOUBLE_QUOTE] = "double quote",
    [TOKEN_SINGLE_QUOTE] = "single quote",
    [TOKEN_HASHTAG] = "hashtag",

    // literals
    [TOKEN_NUMBER] = "number",
    [TOKEN_STRING] = "string",
    [TOKEN_BOOLEAN] = "boolean",
    [TOKEN_COMMENT] = "comment",

    // keywords
    [TOKEN_CHECK] = "check",
    [TOKEN_INPUT] = "input",
    [TOKEN_OUTPUT] = "output",
    [TOKEN_LIST] = "list",
    [TOKEN_TRUE] = "true",
    [TOKEN_FALSE] = "false",

    // keywords for literal type declarations
    [TOKEN_BOOL] = "bool",
    [TOKEN_NUM] = "num",
    [TOKEN_STR] = "str",

    // other
    [TOKEN_ERROR] = "error",
    [TOKEN_KEYWORD_ERROR] = "keyword error",
    [TOKEN_EOF] = "eof",

    // not to be used while scanning
    [TOKEN_NONE] = "none"};

void print_token(Token *token)
{
    printf("%s\n", tokenasstring[token->type]);
}

void print_run_command(char *command)
{
    printf("cmd: %s\n", command);
}