#include "Generators.h"
#include "Platform.h"

#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>
#include <stdbool.h>

#include "Project.h"
#include "Util.h"

static char *PBldDefinesToGccStyle(PBldStringArray defines)
{
    char *result = calloc(1, 1);
    size_t resultLength = 0;

    if (defines.stringCount == 0) {
        return result;
    }

    for (size_t i = 0; i < defines.stringCount; i++) {
        char *token = defines.data[i];

        size_t tokenLength = strlen(token);
        result = realloc(result, resultLength + 2 + tokenLength + 1);
        resultLength = resultLength + 2 + tokenLength;

        strncat(result, "-D", resultLength);
        strncat(result, token, resultLength);
        strncat(result, " ", resultLength);
    }

    return result;
}

static char *PBldIncludesToGccStyle(PBldStringArray includePaths)
{
    char *result = calloc(1, 1);
    size_t resultLength = 0;

    if (includePaths.stringCount == 0) {
        return result;
    }

    for (size_t i = 0; i < includePaths.stringCount; i++) {
        char *token = includePaths.data[i];

        size_t tokenLength = strlen(token);
        result = realloc(result, resultLength + 2 + tokenLength + 1);
        resultLength = resultLength + 2 + tokenLength;

        strncat(result, "-I", resultLength);
        strncat(result, token, resultLength);
        strncat(result, " ", resultLength);
    }

    return result;
}

static char *PBldDependenciesToGccStyleLibs(PBldProjectArray dependencies)
{
    char *result = calloc(1, 1);
    size_t resultLength = 0;

    if (dependencies.projectCount == 0) {
        return result;
    }

    for (size_t i = 0; i < dependencies.projectCount; i++) {
        char *token = dependencies.data[i].projectName;

        size_t tokenLength = strlen(token);
        result = realloc(result, resultLength + 2 + tokenLength + 1);
        resultLength = resultLength + 2 + tokenLength;

        strncat(result, "-l", resultLength);
        strncat(result, token, resultLength);
        strncat(result, " ", resultLength);
    }

    return result;
}

char *PBldDependenciesToFileNames(PBldProjectArray dependencies)
{
    char *string = calloc(1, 1);
    size_t stringSize = 0;

    if (dependencies.projectCount == 0) {
        return string;
    }

    for (size_t i = 0; i < dependencies.projectCount; i++) {
        BldProject *project = &dependencies.data[i];

        char projectPath[128] = {0};
        if (project->type == BLD_EXECUTABLE) {
            snprintf(projectPath, 127, " bin/" PLAT_EXE_TEMPLATE, project->projectName);
        } else if (project->type == BLD_DYNAMIC_LIBRARY) {
            snprintf(projectPath, 127, " bin/" PLAT_DYNLIB_TEMPLATE, project->projectName);
        } else if (project->type == BLD_STATIC_LIBRARY) {
            snprintf(projectPath, 127, " bin/" PLAT_STATICLIB_TEMPLATE, project->projectName);
        } else {
            assert(false);
        }

        stringSize += strlen(projectPath);
        string = realloc(string, stringSize + 1);

        strncat(string, projectPath, stringSize);
    }

    return string;
}

void PBldAddProjectToMakefile(FILE *makefile, BldProject *project)
{
    // TODO: too many allocations :S
    assert(makefile != NULL);
    assert(project != NULL);

    char *cStandardVersion = NULL;
    switch (project->language) {
    case BLD_LANGUAGE_C90:
        cStandardVersion = "-std=c90 ";
        break;
    case BLD_LANGUAGE_C99:
        cStandardVersion = "-std=c99 ";
        break;
    case BLD_LANGUAGE_C11:
        cStandardVersion = "-std=c11 ";
        break;
    case BLD_LANGUAGE_C17:
        cStandardVersion = "-std=c17 ";
        break;
    case BLD_LANGUAGE_C23:
        cStandardVersion = "-std=c23 ";
        break;
    default:
        cStandardVersion = "";
        break;
    }


    char *gccDefines = PBldDefinesToGccStyle(project->defines);
    char *gccIncludes = PBldIncludesToGccStyle(project->includePaths);
    char *gccLibraries = PBldDependenciesToGccStyleLibs(project->dependencies);
    if (project->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile, "%s_CFLAGS=$(CFLAGS) $(CFLAGS_DYNLIB) %s %s %s\n",
                project->projectName,
                gccDefines,
                gccIncludes,
                cStandardVersion);

        fprintf(makefile, "%s_LDFLAGS=$(LDFLAGS) $(LDFLAGS_DYNLIB) %s",
                project->projectName,
                gccLibraries);
        if (project->linkerScript != NULL) {
            fprintf(makefile, " -T %s\n\n", project->linkerScript);
        } else {
            fprintf(makefile, "\n\n");
        }
    } else {
        fprintf(makefile, "%s_CFLAGS=$(CFLAGS) %s %s %s\n",
                project->projectName,
                gccDefines,
                gccIncludes,
                cStandardVersion);

        fprintf(makefile, "%s_LDFLAGS=$(LDFLAGS) %s",
                project->projectName,
                gccLibraries);
        if (project->linkerScript != NULL) {
            fprintf(makefile, " -T%s\n\n", project->linkerScript);
        } else {
            fprintf(makefile, "\n\n");
        }
    }
    free(gccIncludes);
    free(gccDefines);

    PBldStringArray objectFiles = PBldCreateStringArray();
    PBldStringArray dependencyFiles = PBldCreateStringArray();

    for (size_t i = 0; i < project->sources.stringCount; i++) {
        char *sourceFile = project->sources.data[i];

        const size_t sourceFileLength = strlen(basename(sourceFile));
        char *objectFile = calloc(sourceFileLength + 1, 1);
        strncpy(objectFile, basename(sourceFile), sourceFileLength);
        char *extPointer = strrchr(objectFile, '.');
        assert(extPointer != NULL);
        *(extPointer) = '\0';

        char *dependencyFile = calloc(sourceFileLength + 1, 1);
        strncpy(dependencyFile, basename(sourceFile), sourceFileLength);
        extPointer = strrchr(dependencyFile, '.');
        assert(extPointer != NULL);
        *(extPointer) = '\0';

        // Path: "bin/projecname.d/source.o"
        const size_t objectPathLength = strlen("bin/") +
                                        strlen(project->projectName) +
                                        strlen(objectFile) +
                                        6;
        char *objectPath = calloc(objectPathLength + 1, 1);
        snprintf(objectPath,
                 objectPathLength,
                 "bin/%s.d/%s.o",
                 project->projectName,
                 objectFile);

        // Path: "bin/projecname.d/source.d"
        const size_t dependencyPathLength = strlen("bin/") +
                                            strlen(project->projectName) +
                                            strlen(dependencyFile) +
                                            6;
        char *dependencyPath = calloc(dependencyPathLength + 1, 1);
        snprintf(dependencyPath,
                 dependencyPathLength,
                 "bin/%s.d/%s.d",
                 project->projectName,
                 dependencyFile);

        fprintf(makefile, "%s: %s\n", objectPath, sourceFile);
        fprintf(makefile,
                "\t$(CC) -c $(%s_CFLAGS) -o %s %s\n\n",
                project->projectName,
                objectPath,
                sourceFile);

        // Should move instead of appending. That way we do not allocate and
        // free the same string.
        PBldAppendToStringArray(&objectFiles, objectPath);
        PBldAppendToStringArray(&dependencyFiles, dependencyPath);
        free(objectFile);
        free(dependencyFile);
        free(objectPath);
    }

    fprintf(makefile, "%s_OBJ =", project->projectName);
    for (size_t i = 0; i < objectFiles.stringCount; i++) {
        fprintf(makefile, " %s", objectFiles.data[i]);
    }
    fprintf(makefile, "\n");

    fprintf(makefile, "%s_DEP =", project->projectName);
    for (size_t i = 0; i < dependencyFiles.stringCount; i++) {
        fprintf(makefile, " %s", dependencyFiles.data[i]);
    }

    fprintf(makefile, "\n\n");

    char *finalDependencies = PBldDependenciesToFileNames(project->dependencies);
    if (project->type == BLD_EXECUTABLE) {
        fprintf(makefile,
                "bin/" PLAT_EXE_TEMPLATE ": $(%s_OBJ) %s\n",
                project->projectName,
                project->projectName,
                finalDependencies);
        fprintf(makefile,
                "\t$(CC) $(%s_LDFLAGS) $(%s_OBJ) -o bin/" PLAT_EXE_TEMPLATE "\n\n",
                project->projectName,
                project->projectName,
                project->projectName);
    } else if (project->type == BLD_DYNAMIC_LIBRARY) {
        // Workaround with setting the library path direcly to bin/name. For
        // some reason everything stops working.
        if (PLAT_IS_APPLE) {
            fprintf(makefile,
                    "bin/" PLAT_DYNLIB_TEMPLATE ": $(%s_OBJ) %s\n",
                    project->projectName,
                    project->projectName,
                    finalDependencies);
            fprintf(makefile,
                    "\t$(CC) -shared $(%s_LDFLAGS) $(%s_OBJ) -o " PLAT_DYNLIB_TEMPLATE "\n\n",
                    project->projectName,
                    project->projectName,
                    project->projectName);
            fprintf(makefile,
                    "\tmv " PLAT_DYNLIB_TEMPLATE " bin/" PLAT_DYNLIB_TEMPLATE "\n\n",
                    project->projectName,
                    project->projectName);
        } else {
            fprintf(makefile,
                    "bin/" PLAT_DYNLIB_TEMPLATE ": $(%s_OBJ) %s\n",
                    project->projectName,
                    project->projectName,
                    finalDependencies);
            fprintf(makefile,
                    "\t$(CC) -shared $(%s_LDFLAGS) $(%s_OBJ) -o bin/" PLAT_DYNLIB_TEMPLATE "\n\n",
                    project->projectName,
                    project->projectName,
                    project->projectName);
        }
    } else if (project-> type == BLD_STATIC_LIBRARY) {
        fprintf(makefile,
                "bin/" PLAT_STATICLIB_TEMPLATE ": $(%s_OBJ) %s\n",
                project->projectName,
                project->projectName,
                finalDependencies);
        fprintf(makefile,
                "\t$(AR) rcs bin/" PLAT_STATICLIB_TEMPLATE " $(%s_OBJ)\n\n",
                project->projectName,
                project->projectName);
    } else {
        assert(false);
    }

    fprintf(makefile, "-include bin/%s/*.d\n\n", project->projectName);
}

void PBldGenerateMakefile(FILE *makefile,
                          BldProject *defaultTarget,
                          BldProject *projects,
                          size_t projectCount)
{
    assert(makefile != NULL);
    assert(defaultTarget != NULL);
    assert(projects != NULL);

    if (defaultTarget->type == BLD_EXECUTABLE) {
        fprintf(makefile, "all: bin/" PLAT_EXE_TEMPLATE "\n\n", defaultTarget->projectName);
    } else if (defaultTarget->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile, "all: bin/" PLAT_DYNLIB_TEMPLATE "\n\n", defaultTarget->projectName);
    } else if (defaultTarget->type == BLD_STATIC_LIBRARY) {
        fprintf(makefile, "all: bin/" PLAT_STATICLIB_TEMPLATE "\n\n", defaultTarget->projectName);
    }

    fprintf(makefile, "CFLAGS=-Wall -Wextra -MMD\n");
    fprintf(makefile, "CFLAGS_DYNLIB=-fPIC\n");
    fprintf(makefile, "LDFLAGS=-Lbin/\n\n");
    fprintf(makefile, "LDFLAGS_DYNLIB=-fPIC\n\n");

    fprintf(makefile, "TO_CLEAN=\n\n");

    // Add a way to rebuild the makefile if the build script has been changed.
    fprintf(makefile, "Makefile: Build.c\n");
    fprintf(makefile, "\t$(CC) -o Build Build.c -lBuild\n");
    fprintf(makefile, "\t./Build\n\n");

    for (size_t i = 0; i < projectCount; i++) {
        BldProject *project = &projects[i];
        PBldAddProjectToMakefile(makefile, project);

        fprintf(makefile, "TO_CLEAN+=$(%s_OBJ)\n", project->projectName);
        fprintf(makefile, "TO_CLEAN+=$(%s_DEP)\n", project->projectName);

        if (project->type == BLD_EXECUTABLE) {
            fprintf(makefile, "TO_CLEAN+=bin/" PLAT_EXE_TEMPLATE "\n\n", project->projectName);
        } else if (project->type == BLD_DYNAMIC_LIBRARY) {
            fprintf(makefile, "TO_CLEAN+=bin/" PLAT_DYNLIB_TEMPLATE "\n\n", project->projectName);
        } else if (project->type == BLD_STATIC_LIBRARY) {
            fprintf(makefile, "TO_CLEAN+=bin/" PLAT_STATICLIB_TEMPLATE "\n\n", project->projectName);
        }
    }

    fprintf(makefile, "clean:\n");
    fprintf(makefile, "\trm -f $(TO_CLEAN)\n");
}
