#include <stdlib.h>
// #include <stdbool.h>

#include "io.h"

static void init_stream(Stream* stream)
{
    stream->stream = NULL;
    stream->length = 0;
    stream->capacity = 0;
}

size_t write_stream(Stream *stream, const Token token)
{
    if (stream->capacity < stream->length + token.length + 2) // leading space and nul terminator
    {
        stream->capacity = (stream->capacity < 1024) ? 1024 : stream->capacity * ((stream->length + token.length + 2) / 1024 + 1);
        stream->stream = realloc(stream->stream, sizeof(char) * stream->capacity);
        if (stream->stream == NULL) return 0;
    }

    stream->stream[stream->length] = ' ';
    stream->length++;

    for (int i = 0; i < token.length; i++)
    {
        stream->stream[stream->length] = token.start[i];
        stream->length++;
    }

    return token.length + 2;
}

void init_test_case(TestCase *testcase)
{
    testcase->ischeckcase = false;
    init_stream(&testcase->input);
    init_stream(&testcase->output);
}

void init_io(IO *io)
{
    io->capacity = 0;
    io->numtestcases = 0;
    io->testcases = NULL;
}

void free_io(IO *io)
{
    
    init_io(io);
}

bool add_test_case(IO* io, const TestCase* testcase)
{
    if (io->capacity < io->numtestcases + 1)
    {
        io->capacity = (io->capacity < 8) ? 8 : io->capacity * 2;
        io->testcases = realloc(io->testcases, sizeof(TestCase) * io->capacity);
        if (io->testcases == NULL) return false;
    }

    io->testcases[io->numtestcases] = *testcase;
    io->numtestcases++;
    return true;
}