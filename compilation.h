#include <stdbool.h>

#ifndef COMPILATION_H
#define COMPILATION_H

#define MAX_FILE_NAME_LEN 1028

// the list of languages that we can compile
typedef enum
{
    JAVA,
    NONE
} Language;

// 
typedef struct
{
    int filenamelen;
    int pathlen;
    Language language;
    bool cancompile;
    bool canrun;
    char absolutefilepath[MAX_FILE_NAME_LEN];
} File;

// takes a File and string containing the path/name of a source code document. And stores the meta
// data required for compiling and running the file in the File struct so the program can compile
// and run the code.
// returns true if the program has a compiler and runner for the source code
// return false if the program can't identify (compile and run) the source code language
bool init_file(File *file, const char* source);
bool compile_file(File* file);
bool run_file(File* file);

// to remove?
void print_file(File* file);


#endif