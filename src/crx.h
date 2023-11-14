#ifndef AG_CRX_H
#define AG_CRX_H

#include "file.h"
#include "io.h"

typedef enum
{
    COMPILE_OK,
    COMPILE_ERROR,
    RUN_OK,
    RUN_ERROR
} CRXResult;

/*
 * The function compile_file compiles the source code so it can then be run
 * Any messages and stack traces the compiler reports will be logged in a local file "error.txt"
 *
 * @param File* file
 *      the file struct that contains the meta data so the source code can be compiled
 *
 * @returns
 *      
 */
CRXResult compile(File* file);


CRXResult run(File* file, const IO io);

#endif