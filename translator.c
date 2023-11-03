
#include <stdbool.h>
#include <stdio.h>

#include "translator.h"
#include "scanner.h"

typedef struct
{
    Token current;
    Token previous;
    bool haderror;
} Parser;


Parser parser;


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


bool translate(const char* source)
{
    init_scanner(source);

    parser.haderror = false;

    advance();

    while (!match(TOKEN_EOF))
    {
        // sdd
        print_token();
    }

    end_process();
    return !parser.haderror;
}