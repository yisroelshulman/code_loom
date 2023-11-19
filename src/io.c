#include <stdlib.h>

#include "io.h"

// initializes a Stream to its initial values so it can be used
static void init_stream(Stream* stream)
{
    stream->stream = NULL;
    stream->length = 0;
    stream->capacity = 0;
}

// frees the allocated memory to the Stream
void free_stream(Stream* stream)
{
    free(stream->stream);
    init_stream(stream);
}

// writes the token to the Stream returns the number of characters copied to the stream
size_t write_stream(Stream *stream, const Token token)
{
    if (stream->capacity < stream->length + token.length + 2) // leading space and nul terminator
    {
        if (stream->capacity == 0) stream->capacity = 1024; // avoid a buffer overflow when capacity is 0
        stream->capacity = stream->capacity * ((stream->length + token.length + 2) / 1024 + 1);
        stream->stream = realloc(stream->stream, sizeof(char) * stream->capacity);
        if (stream->stream == NULL) return 0;
    }
    // always starts with a space to separate the cl args, if length is not 0 it overwrites the nul
    // terminator from the previous write
    stream->stream[stream->length] = ' ';
    stream->length++;

    for (int i = 0; i < token.length; i++)
    {
        stream->stream[stream->length] = token.start[i];
        stream->length++;
    }

    stream->stream[stream->length] = '\0'; // nul terminate

    return token.length + 2; // leading space + the nul terminate
}

// initializes a TestCase to its initial values
void init_test_case(TestCase *testcase)
{
    testcase->ischeckcase = false;
    init_stream(&testcase->input);
    init_stream(&testcase->output);
}

// frees the allocated memory for the testcase
void free_test_case(TestCase* testcase)
{
    free_stream(&testcase->input);
    free_stream(&testcase->output);
    testcase = NULL;
}

// initialized the IO to its initial values
void init_io(IO *io)
{
    io->capacity = 0;
    io->numtestcases = 0;
    io->testcases = NULL;
}

// frees all the allocated memory to the IO
void free_io(IO *io)
{
    for (int i = 0; i < io->numtestcases; i++)
    {
        free_test_case(&io->testcases[i]);
    }
    free(io->testcases);
    init_io(io);
}

// adds the TestCase to the IO
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