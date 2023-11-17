#ifndef AU_COMPARATOR_H
#define AU_COMPARATOR_H

#include "result.h"

/*
 * The compare function compares two string and determines whether or not they are considered a
 * a match.
 *
 * After a call to compare the user can expect the strings to remain unchanged
 *
 * @param const char* expected
 *      the string we are comparing to
 * @param const char* received
 *      the string being compared
 *
 * @returns
 *      PASS:   if the two strings are considered a match
 *      FAIL:   if the two strings are not considered a match
 */
Status compare(const char* expected, const char* received);

#endif