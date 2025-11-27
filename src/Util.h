#ifndef UTIL_H
#define UTIL_H

#include <string.h>

typedef struct
{
    char **data;
    size_t stringCount;
} PBldStringArray;

PBldStringArray PBldCreateStringArray();

void PBldAppendToStringArray(PBldStringArray *sa, char *string);

#endif // UTIL_H
