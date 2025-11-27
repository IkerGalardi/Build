#ifndef UTIL_H
#define UTIL_H

#include <string.h>

typedef struct BldProject BldProject;

typedef struct
{
    char **data;
    size_t stringCount;
} PBldStringArray;

typedef struct
{
    BldProject *data;
    size_t projectCount;
} PBldProjectArray;

PBldStringArray PBldCreateStringArray();

void PBldAppendToStringArray(PBldStringArray *sa, char *string);

PBldProjectArray PBldCreateProjectArray();

void PBldAppendToProjectArray(PBldProjectArray *pa, BldProject *project);

#endif // UTIL_H
