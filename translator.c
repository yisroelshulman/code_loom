
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "translator.h"
//#include "scanner.h"
//#include "io.h"

typedef struct
{
    Token current;
    Token previous;
    bool haderror;
    bool panicmode;
    TestCase testcase;
    bool isaddable;
} Parser;


Parser parser;
IO* translatingio;


static void error(Token* token, const char* message)
{
    if (parser.panicmode) return;
    parser.panicmode = true;

    parser.isaddable = false;

    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF)
    {
        fprintf(stderr, " at end");
    }
    else
    {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, " : %s\n", message);
    parser.haderror = true;
}

static void advance()
{
    parser.previous = parser.current;

    for (;;)
    {
        parser.current = scan_token();
        if (parser.current.type == TOKEN_COMMENT) continue;
        if (parser.current.type != TOKEN_ERROR) break;

        error(&parser.current, parser.current.start);
    }
}

static bool check(TokenType type)
{
    return parser.current.type == type;
}

static bool match(TokenType type)
{
    if (!check(type)) return false;
    advance();
    return true;
}

static void consume(TokenType type, const char* message)
{
    if (parser.current.type == type)
    {
        advance();
        return;
    }
    error(&parser.current, message);
}

static void synchronize()
{
    parser.panicmode = false;

    while (parser.current.type != TOKEN_EOF)
    {
        if (parser.previous.type == TOKEN_RIGHT_BRACE) return;
        switch(parser.current.type)
        {
            case TOKEN_LEFT_BRACE:
            case TOKEN_CHECK:
                return;

            default:
                break;
        }
    }
}

static void number(canbelist)
{

}

static void string(bool canbelist)
{

}

static void boolean(bool canbelist)
{

}

static void input()
{
    advance();
    switch (parser.previous.type)
    {
        case TOKEN_NUM:
            consume(TOKEN_COLON, "Expected ':' after data type declaration.\n");
            number(true);
            break;
        case TOKEN_STR:
            consume(TOKEN_COLON, "Expected ':' after data type declaration.\n");
            string(true);
            break;
        case TOKEN_BOOL:
            consume(TOKEN_COLON, "Expected ':' after data type declaration.\n");
            boolean(true);
            break;
        default:
            error(&parser.previous, "Data type expected.\n");

    }
    consume(TOKEN_RIGHT_PARENT, "Expected ')' to end the input.\n");
}

static void output()
{

}

static void block()
{
    consume(TOKEN_INPUT, "Expected 'input' at the start of a block.\n");
    consume(TOKEN_LEFT_PARENT, "Expected '(' to start an input statement.\n");
    input();
    consume(TOKEN_OUTPUT, "Expected 'output' after input statement.\n");
    consume(TOKEN_LEFT_PARENT, "Expected (' to start an output statement.\n");
    output();
    consume(TOKEN_RIGHT_BRACE, "Expected '}' to end a test case.\n");
}

static void test_case()
{
    TestCase testcase;
    parser.testcase = testcase;
    parser.testcase.ischeckcase = match(TOKEN_CHECK);
    parser.isaddable = true;
    consume(TOKEN_LEFT_BRACE, "Expected '{' to start a test case.\n"); 

    block();

    if (parser.isaddable) add_test_case(&parser.testcase);
    if (parser.panicmode) synchronize();
    return;
}


static bool translate(const char* source, IO* io)
{
    init_scanner(source);

    translatingio = io;

    parser.haderror = false;
    parser.panicmode = false;

    advance();

    while (!match(TOKEN_EOF))
    {
        test_case();
        // sdd
        // print_token(&parser.current);
        // advance();
    }

    print_token(&parser.current);

    return !parser.haderror;
}

static char* read_file(const char* path)
{

    FILE* file = fopen(path, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\"\n", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(filesize + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Not enough memory to read \"%s\"\n", path);
        return NULL;
    }

    size_t bytesread = fread(buffer, sizeof(char), filesize, file);
    if (bytesread < filesize)
    {
        fprintf(stderr, "Could not read file \"%s\"\n", path);
        return NULL;
    }

    buffer[bytesread] = '\0'; // nul terminate
    return buffer;
}

TranslateResult translate_file(const char* path, IO* io)
{
    char* source = read_file(path);
    if (source == NULL)
    {
        return READ_ERROR;
    }
    bool result = translate(source, io);
    free(source);

    if (!result)
    {
        return TRANSLATION_ERROR;
    }
    return SUCCESS;
}


// =================================================================================================
// to remove
// =================================================================================================

char *ptoken[] = {
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
    [TOKEN_EOF] = "eof"

};

void print_token(Token* token)
{
    printf("%s\n", ptoken[token->type]);
}