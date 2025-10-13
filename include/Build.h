#ifndef BUILD_H
#define BUILD_H

#include <stdbool.h>

typedef enum
{
    BLD_EXECUTABLE,
    BLD_STATIC_LIBRARY,
    BLD_DYNAMIC_LIBRARY,
} BldProjectType;

typedef enum
{
    BLD_LANGUAGE_DEFAULT,
    BLD_LANGUAGE_C89,
    BLD_LANGUAGE_C90,
    BLD_LANGUAGE_C99,
    BLD_LANGUAGE_C11,
    BLD_LANGUAGE_C17,
    BLD_LANGUAGE_C23
} BldLanguage;

typedef struct
{
    char projectName[64];
    BldProjectType type;
    char *sources;
    char *includePaths;
    BldLanguage language;
} BldProject;

BldProject *BldNewProject(char *projectName,
                          BldProjectType type,
                          BldLanguage language);

void BldAddSources(BldProject *project, ...);

void BldAddIncludePaths(BldProject *project, ...);

void BldAddDependencies(BldProject *project, ...);

void BldGenerate(BldProject *defaultTarget);

#endif // BUILD_H
