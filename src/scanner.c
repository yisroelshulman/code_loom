#include <stdbool.h>
#include <string.h>

#include "scanner.h"

// a token scanner that keeps a pointer to the start of the token and a pointer to the current
// position in the text file. finally the line in the document where the token can be found for
// better error reporting
typedef struct
{
    const char* start;
    const char* current;
    int line;
} Scanner;

// the scanner only one scanner
Scanner scanner;

// initialize the scanner to point to the start of the source code and on the first line in the file
void init_scanner(const char* source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

// returns true if the scanner is at the end of the file
// returns false otherwise
static bool is_at_end()
{
    return *scanner.current == '\0';
}

// returns true if the scanner is looking at a letter in the english alphabet
// returns false otherwise
static bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// returns true if the scanner is looking at a digit 0 - 9
// returns false otherwise
static bool is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

// returns true is the character is a '-' symbol
// returns false otherwise
static bool is_minus(char c)
{
    return c == '-';
}

// returns the character the scanner is currently looking at
static char peek()
{
    return *scanner.current;
}

// moves the scanner forward by one character and returns the previous character
static char advance()
{
    scanner.current++;
    return scanner.current[-1];
}

// skips the whitespaces since they have no meaning in SUL
// if a new line character is seen the line number is incremented for error reporting
static void skip_whitespace()
{
    for (;;)
    {
        char c = peek();
        switch (c)
        {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            default:
                return;
        }
    }
}

// creates a token of type type and returns it
static Token make_token(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = scanner.current - scanner.start;
    token.line = scanner.line;
    return token;
}

// creates a error token where the contents of the token is the error message
static Token error_token(const char* message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message); // message is always nul terminated
    token.line = scanner.line;
    return token;
}

// checks if the remaining letters makes a keyword, return the keyword token if successful otherwise
// returns an error token since SUL does not contain any variables.
static TokenType check_keyword(int start, int length, const char* rest, TokenType type)
{
    if ((scanner.current - scanner.start == start + length) && (memcmp(scanner.start + start, rest, length) == 0)) return type;

    return TOKEN_ERROR;
}

// tries to scan and return a keyword token if not a keyword token an error token is returned
static TokenType keyword_type()
{
    switch (scanner.start[0])
    {
        case 'i': return check_keyword(1, 4, "nput", TOKEN_INPUT);
        case 'o': return check_keyword(1, 5, "utput", TOKEN_OUTPUT);
        case 's': return check_keyword(1, 2, "tr", TOKEN_STR);
        case 'n': return check_keyword(1, 2, "um", TOKEN_NUM);
        case 'b': return check_keyword(1, 3, "ool", TOKEN_BOOL);
        case 'l': return check_keyword(1, 3, "ist", TOKEN_LIST);
        case 't': return check_keyword(1, 3, "rue", TOKEN_BOOLEAN);
        case 'f': return check_keyword(1, 4, "alse", TOKEN_BOOLEAN);
        case 'c': return check_keyword(1, 4, "heck", TOKEN_CHECK);
    }

    return TOKEN_ERROR;
}

//scans the characters of the token that might be a keyword then calls keyword_type to verify
static Token keyword()
{
    while (is_alpha(peek())) advance();
    TokenType type = keyword_type();
    if (type == TOKEN_ERROR) return error_token("Unexpected character.");
    return make_token(type);
}

// scans and returns a token of the type number
// numbers have the form [0..9]+.[0..9*]
static Token number()
{
    if (is_minus(peek())) advance(); // negative

    while (is_digit(peek())) advance();

    if (peek() == '.')
    {
        advance(); // consume the '.'

        while (is_digit(peek())) advance();
    }

    return make_token(TOKEN_NUMBER);
}

// scans and returns a token of the type string
// strings have one of two forms "[^""]" or '[^']'
static Token string(const char quote, TokenType type)
{
    while (peek() != quote && !is_at_end())
    {
        // multiline strings are allowed
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (is_at_end()) return error_token("Unterminated string.");
    advance(); // consume closing quote
    return make_token(TOKEN_STRING);
}

// scans and returns a comment token
// token have the form #....
static Token line_comment()
{
    while (peek() != '\n' && !is_at_end()) advance();
    return make_token(TOKEN_COMMENT);
}

// scans and returns the next token in the sourcecode file
Token scan_token()
{
    skip_whitespace();
    scanner.start = scanner.current;

    if (is_at_end()) return make_token(TOKEN_EOF);

    char c = advance();
    if (is_alpha(c)) return keyword();
    if (is_digit(c)) return number();

    switch (c)
    {
        case '(': return make_token(TOKEN_LEFT_PARENT);
        case ')': return make_token(TOKEN_RIGHT_PARENT);
        case '{': return make_token(TOKEN_LEFT_BRACE);
        case '}': return make_token(TOKEN_RIGHT_BRACE);
        case '[': return make_token(TOKEN_LEFT_BRACKET);
        case ']': return make_token(TOKEN_RIGHT_BRACKET);
        case ':': return make_token(TOKEN_COLON);
        case '"': return string('"', TOKEN_DOUBLE_QUOTE);
        case '\'': return string('\'', TOKEN_SINGLE_QUOTE);
        case '#': return line_comment();
        case '-': return number();
    }
    return error_token("Unexpected character.");
}