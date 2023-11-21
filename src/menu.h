#ifndef AG_MENU_H
#define AG_MENU_H

typedef enum
{
    SELECTION,
    RUN,
    SUBMIT_PROMPT,
    SUBMIT_MENU,
    PAUSE
} Menu;

typedef enum
{
    HELP,
    DEFAULT_PROBLEM,
    PROBLEM_LIST,
    EXIT,
    CHECK,
    SUBMIT,
    ADD_TEST_CASE,
    BACK,
    REPEAT,
    INVALID
} Selection;

Selection menu(Menu menu);



#endif