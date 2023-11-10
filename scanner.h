#ifndef AG_SUL_SCANNER_H
#define AG_SUL_SCANNER_H

// TokenType is all the types of tokens that are recognized by the SUL language
typedef enum
{
    // symbols
    TOKEN_LEFT_PARENT,
    TOKEN_RIGHT_PARENT,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_COLON,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_SINGLE_QUOTE,
    TOKEN_HASHTAG,

    // literals
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_BOOLEAN,
    TOKEN_COMMENT,

    // keywords
    TOKEN_CHECK,
    TOKEN_INPUT,
    TOKEN_OUTPUT,
    TOKEN_LIST,
    TOKEN_TRUE,
    TOKEN_FALSE,

    // keywords for literal type declarations
    TOKEN_BOOL,
    TOKEN_NUM,
    TOKEN_STR,

    // other
    TOKEN_ERROR,
    TOKEN_EOF,

    // not to be used while scanning
    TOKEN_NONE

} TokenType;

/*
 * The token struct represents a token in SUL
 *
 * Fields:
 * - TokenType type
 *      the type of token it is
 * - const char* start
 *      a pointer to the start of the token in the file
 * - int length
 *      the character count of the token
 * - int line
 *      the line in the source code where the token is
 */
typedef struct
{
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

// functions
// -------------------------------------------------------------------------------------------------

/*
 * The init_scanner function creates a scanner that is associated with a source code file the
 * scanner is initiated and points to the start of the file and is ready to scan tokens.
 *
 * after a successful call to init_scanner the user can expect the scanner to point to the start of
 * the source code and the scanner ready to scan tokens
 *
 * Note: Only one scanner is available if the user wants to use a second source code file the
 * scanner needs to be reinitialized
 *
 * @params const char* source
 *      the stream containing the source code
 */
void init_scanner(const char* source);

/*
 * The scan_token function scans a single token from the source code and returns it
 *
 * @ returns
 *      the token that was scanned
 *      on failure the token returned is an error token that contains the error message
 */
Token scan_token();

#endif