#ifndef PROJECT_H
#define PROJECT_H

#include "Build.h"
#include "Util.h"

typedef struct BldProject
{
    char projectName[64];
    BldProjectType type;
    PBldStringArray sources;
    PBldStringArray includePaths;
    PBldStringArray defines;
    PBldStringArray dependencies;
    BldLanguage language;
    char *linkerScript;
} BldProject;

#endif // PROJECT_H
