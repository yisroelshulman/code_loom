
#include <stdbool.h>

#include "translator.h"
#include "scanner.h"

typedef struct
{
    Token current;
    Token previous;
    bool haderror;
    bool panicmode;
} Parser;


Parser parser;



static void advance()
{
    parser.previous = parser.current;

    for (;;)
    {
        parser.current = scan_token();
        if (parser.current.type == TOKEN_COMMENT) continue;
        if (parser.current.type != TOKEN_ERROR) break;

        error_at_current(parser.current.start);
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


bool convert(const char* source)
{
    init_scanner(source);

    parser.haderror = false;
    parser.panicmode = false;

    advance();

    while (!match(TOKEN_EOF))
    {
        // sdd
        print_token();
    }

    end_process();
    return !parser.haderror;
}