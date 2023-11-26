#ifndef AG_DEBUG_H
#define AG_DEBUG_H

#include "io.h"
#include "scanner.h"

//#define DEBUG_MODE

void print_input(Stream *stream);
void print_io(IO* io);
void print_run_command(char* command);
void print_token(Token *token);

#endif