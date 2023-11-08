#include <stdlib.h>

#include "io.h"

void init_stream(Stream* stream)
{
    stream->stream = NULL;
    stream->length = 0;
    stream->capacity = 0;
}

size_t write_stream(Stream *stream, const Token token)
{
    if (stream->capacity < stream->length + token.length + 2) // leading space and nul terminator
    {
        stream->capacity = stream->capacity * ((stream->length + token.length + 2) / 1028 + 1);
        stream->stream = realloc(stream->stream, sizeof(char) * stream->capacity);
        if (stream->stream == NULL) return 0;
    }

    stream->stream[stream->length] = ' ';
    stream->length++;

    for (int i = 0; i < token.length; i++)
    {
        stream->stream[stream->length + i] = token.start[i];
        stream->length++;
    }
    stream->stream[stream->length] = '\0';

    return token.length + 2;
}

void init_io(IO *io);
void free_io(IO *io);