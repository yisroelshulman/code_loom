#ifndef AG_SUL_TRANSLATOR_H
#define AG_SUL_TRANSLATOR_H

#include "scanner.h"

typedef enum
{
    SUCCESS,
    READ_ERROR,
    TRANSLATION_ERROR
} TranslateResult;

// functions
// -------------------------------------------------------------------------------------------------

/*
 * 
 */
TranslateResult translate_file(const char* path);

// temp functions to test to remove
// -------------------------------------------------------------------------------------------------
// all the tokens of the special use language (domain specific)

void print_token(Token* token);

#endif