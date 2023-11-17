#include <stdlib.h>

#include "io.h"

static void init_stream(Stream* stream)
{
    stream->stream = NULL;
    stream->length = 0;
    stream->capacity = 0;
}

void free_stream(Stream* stream)
{
    free(stream->stream);
    init_stream(stream);
}

size_t write_stream(Stream *stream, const Token token)
{
    if (stream->capacity < stream->length + token.length + 2) // leading space and nul terminator
    {
        if (stream->capacity == 0) stream->capacity = 1024;
        stream->capacity = stream->capacity * ((stream->length + token.length + 2) / 1024 + 1);
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

    stream->stream[stream->length] = '\0';

    return token.length + 2;
}

void init_test_case(TestCase *testcase)
{
    testcase->ischeckcase = false;
    init_stream(&testcase->input);
    init_stream(&testcase->output);
}

void free_test_case(TestCase* testcase)
{
    free_stream(&testcase->input);
    free_stream(&testcase->output);
    testcase = NULL;
}

void init_io(IO *io)
{
    io->capacity = 0;
    io->numtestcases = 0;
    io->testcases = NULL;
}

void free_io(IO *io)
{
    for (int i = 0; i < io->numtestcases; i++)
    {
        free_test_case(&io->testcases[i]);
    }
    free(io->testcases);
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