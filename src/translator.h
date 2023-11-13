#ifndef AG_SUL_TRANSLATOR_H
#define AG_SUL_TRANSLATOR_H

#include "scanner.h"
#include "io.h"

// TranslateResult 
typedef enum
{
    SUCCESS,
    READ_ERROR,
    TRANSLATION_ERROR
} TranslateResult;

// functions
// -------------------------------------------------------------------------------------------------

/*
 * Trasnlate is the function that is responsible for translating a .sul file into test cases and
 * string the test case in the io.
 * 
 * After a successful call to translate the caller can expect the io to contain the all the test 
 * cases.
 * If there was a read error then io will not be modified in any way.
 * If there are any errors during the translation the test case in which the error occurred will not
 * be added to the io
 * 
 * @param const char* path
 *      the path to the .sul file that we want to translate
 * @param IO* io
 *      the io where the test cases will be stored
 * 
 * @returns
 *      SUCCESS:            if no error occurred during the translation
 *      READ_ERROR:         if the .sul file is not found or there was an error reading the file
 *      TRANSLATION_ERROR:  if an error occurred during the translation process
 */
TranslateResult translate(const char* path, IO* io);

 // ==========================================================================================================================================================
// temp functions to test to remove
// ==========================================================================================================================================================

void print_token(Token* token);
void print_io(IO *io);
void print_case(TestCase *testcase);
void print_input(Stream *stream);

#endif