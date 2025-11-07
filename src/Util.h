#ifndef UTIL_H
#define UTIL_H

#include <string.h>

typedef struct
{
    char **data;
    size_t stringCount;
} UtilStringArray;

UtilStringArray UtilCreateStringArray();

void UtilAppendToStringArray(UtilStringArray *sa, char *string);

#endif // UTIL_H
