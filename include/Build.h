#ifndef BUILD_H
#define BUILD_H

#include <stdbool.h>
#include <stdlib.h>

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

typedef struct BldProject BldProject;


BldProject *BldNewProject(char *projectName,
                          BldProjectType type,
                          BldLanguage language);

#define BldSetSources(project, ...) PBldSetSources(project, __VA_ARGS__, NULL)
void PBldSetSources(BldProject *project, ...);

#define BldSetIncludePaths(project, ...) PBldSetIncludePaths(project, __VA_ARGS__, NULL)
void PBldSetIncludePaths(BldProject *project, ...);

#define BldSetDependencies(project, ...) PBldSetDependencies(project, __VA_ARGS__, NULL)
void PBldSetDependencies(BldProject *project, ...);

#define BldSetDefines(project, ...) PBldSetDefines(project, __VA_ARGS__, NULL)
void PBldSetDefines(BldProject *project, ...);

void BldGenerate(BldProject *defaultTarget);

#endif // BUILD_H
