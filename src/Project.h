#ifndef PROJECT_H
#define PROJECT_H

#include "../include/Build.h"

typedef struct BldProject
{
    char projectName[64];
    BldProjectType type;
    char *sources;
    char *includePaths;
    char *defines;
    BldLanguage language;
} BldProject;

#endif // PROJECT_H
