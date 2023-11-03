#include <stdbool.h>

#ifndef AG_FILE_H
#define AG_FILE_H

#define MAX_FILE_NAME_LEN 1028

// Language is the list of languages the program can compile
// by default the language will be NONE
typedef enum
{
    JAVA,
    NONE
} Language;

/*
 * File is the struct that contains the meta data needed to compile and run a source code document
 *
 * Fields:
 * - int pathlen
 *      the length of the path to the subdirectory where the source code file is located
 * - int filenamelen
 *      the length of the file name without the extension
 * - Language language
 *      the programming language of the source code
 * - bool compiled
 *      whether or not the source code compiled successfully
 * - char absolutefilepath[MAX_FILE_NAME_LEN]
 *      the path and name of the source code document
 *
 * Note: pathlen + filenamelen = the len of the absolute file path
 */
typedef struct
{
    int pathlen;
    int filenamelen;
    Language language;
    bool compiled;
    char absolutefilepath[MAX_FILE_NAME_LEN];
} File;

// functions
// -------------------------------------------------------------------------------------------------

/*
 * The function init_file initializes a File with the meta data for a source code document used to
 * compile and run the source code
 *
 * After a successful call to init_file the caller can expect the File to be populated with the
 * correct meta data and compilable
 *
 * @param File* file
 *      the File struct in which to store the meta data
 * @param const char* source
 *      the string containing the path, name, and extension of the source code
 *
 * @returns
 *      true:   if the source code is a valid file type and can be recognized by the compiler
 *      false:  if the source code is not a valid file type
 */
bool init_file(File *file, const char* source);

/*
 * The function compile_file compiles the source code so it can then be run
 * Any messages and stack traces the compiler reports will be logged in a local file "error.txt"
 *
 * @param File* file
 *      the file struct that contains the meta data so the source code can be compiled
 *
 * @returns
 *      true:   if the source code compiled
 *      false:  if the source code did not compile
 */
bool compile_file(File* file);


bool run_file(File* file);

// testing functions to be removed
// =================================================================================================
void print_file(File* file);


#endif
