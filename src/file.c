#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

// does a byte by byte comparison of len bytes from str1 and str2 starting at the start byte of str1
// return true on a match and false otherwise
static bool match_string(const char *str1, const char *str2, int start, int len)
{
    return memcmp(str1 + start, str2, len) == 0;
}

// takes a string and a start position that is one character after a '.' to check if the string is
// a file name with an extention type that we can compile
// returns the file type on a match and NONE otherwise
static Language get_lang(const char *ext, int start)
{
    switch (ext[start])
    {
        case 'j':
            if (start + 5 < MAX_FILE_NAME_LEN)
            {
                if (match_string(ext, "ava\0", start + 1, 4))
                    return JAVA;
            }
            break;
        default:
            break;
    }
    return NONE;
}

// takes a File and string containing the path/name of a source code document. And stores the meta
// data required for compiling and running the file in the File struct so the program can compile
// and run the code.
// returns true if the program has a compiler and runner for the source code
// return false if the program can't identify (compile and run) the source code language
bool init_file(File *file, const char* source)
{
    file->filenamelen = 0;
    file->pathlen = 0;
    file->language = NONE;
    file->compiled = false;
    while (file->filenamelen < MAX_FILE_NAME_LEN && source[file->filenamelen] != '\0')
    {
        // ensures the file name is non empty
        if (file->filenamelen > file->pathlen && source[file->filenamelen] == '.')
        {
            file->language = get_lang(source, file->filenamelen + 1);
            if (file->language != NONE)
            {
                file->absolutefilepath[file->filenamelen] = '\0'; // nul terminate the stirng
                file->filenamelen++;
                return true; // extention recognized
            }
        }
        if (source[file->filenamelen] == '/')
            file->pathlen = file->filenamelen + 1;
        file->absolutefilepath[file->filenamelen] = source[file->filenamelen];
        file->filenamelen++;
    }
    // clean up the file so no further operation run on the file
    file->pathlen = 0;
    file->filenamelen = 0;
    file->absolutefilepath[0] = '\0';
    return false; // extention not recognized
}

// converts the file found at path into a char* buffer. The new buffer is always nul terminated
// which signals the end of the file. If an error occurs during this process the function returns
// NULL otherwise a pointer to the new buffer is returned
char* read_file(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\"\n.", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(filesize + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    }

    size_t bytesread = fread(buffer, sizeof(char), filesize, file);
    if (bytesread < filesize)
    {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        return NULL;
    }

    fclose(file);
    buffer[bytesread] = '\0';
    return buffer;
}

// =================================================================================================
// to be removed
// =================================================================================================

static void print_lang(File* file)
{
    switch (file->language)
    {
        case JAVA:
            printf("java doc\n");
            break;
        case NONE:
            printf("no file ext\n");
            break;
        default:
            printf("default no file ext\n");
            break;
    }
}

static void print_fname(File *file)
{
    for (int i = file->pathlen; i < file->filenamelen - 1; i++)
            printf("%c", file->absolutefilepath[i]);
    printf("\n");
}

static void print_pname(File *file)
{
    if (file->pathlen == 0)
    {
        printf("no file path\n");
        return;
    }
    for (int i = 0; i < file->pathlen; i++)
            printf("%c", file->absolutefilepath[i]);
    printf("\n");
}

void print_file(File* file)
{
    print_pname(file);
    print_fname(file);
    print_lang(file);
}

