
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "translator.h"

// The parser parses the .sul file so it can be translated into test cases and stored in the io
// it keeps track of the current scanned token and the previous scanned token. Additionally whether
// or not there were any errors so the user knows that not all test cases were translated
// The panicmode is set to let the translator know that there was an error while parsing in a block
// and the translator needs to synchronize and advance to the next block so it can continue
// translating.
// the input and output are stored in the parsers testcase before being added to the translating io
// isaddable resets with every new testcase to make sure each block can be translated
typedef struct
{
    Token current;
    Token previous;
    bool haderror;
    bool panicmode;
    TestCase* testcase;
    bool isaddable;
} Parser;

// the parser and io that is used during the translation
Parser parser;
IO* translatingio;

// error handles the errors that occur during the translation of the .sul file and reports the line
// in the .sul file where the error is and an accompanying message on the error
// if the translator is in panicmode no reporting is done so we can avoid cascading errors
// when an error occurs panicmode is set to true and isaddable is set to false and haderror is set
// to true
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


// the advance function moves the parser ahead to the next token ignoring comments
// this is where syntax errors are reported
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

// checks if the the token is the correct token type
static bool check(TokenType type)
{
    return parser.current.type == type;
}

// match advances the parser if the token is the correct type
static bool match(TokenType type)
{
    if (!check(type)) return false;
    advance();
    return true;
}

// consume advances the parser if the token matches otherwise an error is created with the message
// passed as a parameter
static void consume(TokenType type, const char* message)
{
    if (parser.current.type == type)
    {
        advance();
        return;
    }
    error(&parser.current, message);
}

// synchronize advances the scanner until the next block so we can continue translating
// it resets panicmode to false and scans until it reaches the end of a block, the start of a new
// block or the end of the file
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
        advance();
    }
}

// helper method for the input and output functions to get the type of data for the input and output
static TokenType get_data_type()
{
    switch (parser.current.type)
    {
        case TOKEN_NUM:        return TOKEN_NUMBER;
        case TOKEN_STR:        return TOKEN_STRING;
        case TOKEN_BOOL:       return TOKEN_BOOLEAN;
        default:               break;
    }
    return TOKEN_NONE;
}

// adds the contents of the current token to the stream that was passed as an argument
// if an error ocurred during the writing an error is raised
static bool add_to_stream(Stream* stream)
{
    if (write_stream(stream, parser.previous) == 0)
    {
        error(&parser.previous, "Could't add to stream.\n");
        return false;
    }
    return true;
}

// parses the test case input and adds them to the input stream
// inputs can be a lists or individual items
static void input()
{
    TokenType datatype = get_data_type();
    advance();
    consume(TOKEN_COLON, "Expected ':' after data type declaration.\n");

    if (match(TOKEN_LIST))
    {
        consume(TOKEN_LEFT_BRACKET, "Expected '[' to start a list.\n");
        while (match(datatype) && add_to_stream(&parser.testcase->input));
        if (parser.testcase->input.length == 0) error(&parser.current, "Expected at least one list input.\n");
        consume(TOKEN_RIGHT_BRACKET, "Expected ']' to end the list.\n");
    }
    else
    {
        if (!match(datatype) || !add_to_stream(&parser.testcase->input))
        {
            error(&parser.current, "Expected at least one input.\n");
        }
    }
    consume(TOKEN_RIGHT_PARENT, "Expected ')' to end the input.\n");
}

// parses the testcase output and adds it to the output stream
static void output()
{
    TokenType datatype = get_data_type();
    advance();
    consume(TOKEN_COLON, "Expected ':' after data type declaration.\n");

    if (!match(datatype) || !add_to_stream(&parser.testcase->output))
    {
        error(&parser.current, "Expected at least one output.\n");
    }
    consume(TOKEN_RIGHT_PARENT, "Expected ')' to end the output.\n");
}

// parses a testcase block and splits it into the input and the output
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

// parses a testcase and if no error occurred during the translation the test case is added to the
// io so it can be used when running the program
static void test_case()
{
    TestCase testcase;
    init_test_case(&testcase);
    parser.testcase = &testcase;
    parser.testcase->ischeckcase = match(TOKEN_CHECK);
    parser.isaddable = true;
    consume(TOKEN_LEFT_BRACE, "Expected '{' to start a test case.\n"); 

    block();

    if (parser.isaddable) add_test_case(translatingio, parser.testcase);
    if (parser.panicmode) synchronize();
    return;
}

// translates the source into the io's test cases
// returns true or if there were no errors and false if there were
static bool translating(const char* source, IO* io)
{
    init_scanner(source);
    translatingio = io;
    parser.haderror = false;
    parser.panicmode = false;
    advance();

    while (!match(TOKEN_EOF))
    {
        test_case();
    }
    return !parser.haderror;
}

// takes a path to a .sul file and an io and translates the .sul file into test cases and adds them
// to the io so the the test cases can be run on execution
TranslateResult translate(const char* source, IO* io)
{
    if (source == NULL)
    {
        fprintf(stderr, "Invalid source file\n");
        return READ_ERROR;
    }

    bool result = translating(source, io);

    // =====================================================================================================================================================
    print_io(translatingio); // remove

    if (!result)
    {
        return TRANSLATION_ERROR;
    }
    return SUCCESS;
}


// =========================================================================================================================================================
// to remove
// =================================================================================================

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
    [TOKEN_EOF] = "eof",

    // not to be used while scanning
    [TOKEN_NONE] = "none"};

void print_token(Token* token)
{
    printf("%s\n", tokenasstring[token->type]);
}

void print_input(Stream *stream)
{
    printf("input: %s\n", stream->stream);
}

void print_output(Stream *stream)
{
    printf("output: %s\n", stream->stream);
}

void print_case(TestCase* testcase)
{
    printf("is test case? %d\n", testcase->ischeckcase);
    print_input(&testcase->input);
    print_output(&testcase->output);
}

void print_io(IO* io)
{
    printf("cap = %d, count = %d\n\n", io->capacity, io->numtestcases);
    for (int i = 0; i < io->numtestcases; i ++)
    {
        printf("test case %d:\n------------------\n", i);
        print_case(&io->testcases[i]);
        printf("\n");
    }
}