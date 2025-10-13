#ifndef BUILD_H
#define BUILD_H

#include <stdbool.h>

typedef enum
{
    BLD_EXECUTABLE,      /// Runnable program
    BLD_STATIC_LIBRARY,  /// Static library
    BLD_DYNAMIC_LIBRARY, /// Dynamically linked library
    BLD_LIBRARY,         /// Dynamically linked and static library
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

/**
 * @brief Create an executable project given a name and language.
 *
 * @param projectName: name of the project, will be used for the executable
 * @param language: name of the project, will be used for the executable
 * @return an executable handle.
 */
BldProject *BldNewProject(char *projectName,
                          BldProjectType type,
                          BldLanguage language);

/**
 * @brief Add sources to the project
 *
 * @param project: project
 * @param ...: variadic argument of strings containing the names of the files to be
 *             compiled. Last parameter must be NULL.
 */
void BldAddSources(BldProject *project,
                   ...);

/**
 * @brief Add include paths for the project
 *
 * @param project: project
 * @param ...: variadic argument of strings containing the paths to be usable for
 *             includes.
 */
void BldAddIncludePaths(BldProject *project,
                        ...);

/**
 * @brief Add dependencies to the project
 *
 * @param project: project
 * @param ...: variadic argument of strings containing the paths to be usable for
 *             includes.
 */
void BldAddDependencies(BldProject *project,
                        ...);

/**
 * @brief Generate the project.
 */
void BldGenerate(BldProject *defaultTarget);

#endif // BUILD_H
