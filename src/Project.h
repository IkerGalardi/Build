#ifndef PROJECT_H
#define PROJECT_H

#include "Build.h"
#include "Util.h"

typedef struct BldProject
{
    char projectName[64];
    BldProjectType type;
    UtilStringArray sources;
    UtilStringArray includePaths;
    UtilStringArray defines;
    UtilStringArray dependencies;
    BldLanguage language;
    char *linkerScript;
} BldProject;

#endif // PROJECT_H
