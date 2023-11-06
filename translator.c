
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
} Parser;


Parser parser;
IO* translatingio;


static void error(Token* token, const char* message)
{
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

static void block(TestCase* testcase)
{
    if (!match(TOKEN_LEFT_BRACE))
    {

    }
    input();
    output();
}

static void test_case()
{
    TestCase testcase;
    testcase.ischeckcase = match(TOKEN_CHECK);
    if (testcase.ischeckcase)
    {
        consume(TOKEN_COLON, "Expected ':' after check declaration.\n");
    }

    block(&test_case);

    add_test_case(&testcase);
    return;
}


static bool translate(const char* source, IO* io)
{
    init_scanner(source);

    translatingio = io;

    parser.haderror = false;

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

char* ptoken[] = {
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
    [TOKEN_BOOL] = "bool",
    [TOKEN_NUM] = "num",
    [TOKEN_STR] = "str",
    [TOKEN_LIST] = "list",
    [TOKEN_TRUE] = "true",
    [TOKEN_FALSE] = "false",

    // other
    [TOKEN_ERROR] = "error",
    [TOKEN_EOF] = "eof"

};

void print_token(Token* token)
{
    printf("%s\n", ptoken[token->type]);
}