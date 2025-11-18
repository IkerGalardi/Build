#include "Generators.h"
#include "Platform.h"

#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>

#include "Project.h"
#include "Util.h"

static char *PBldDefinesToGccStyle(UtilStringArray defines)
{
    char *result = NULL;
    size_t resultLength = 0;

    if (defines.stringCount == 0) {
        result = malloc(1);
        *result = '\0';
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

static char *PBldIncludesToGccStyle(UtilStringArray includePaths)
{
    char *result = NULL;
    size_t resultLength = 0;

    if (includePaths.stringCount == 0) {
        result = malloc(1);
        *result = '\0';
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
    if (project->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile, "%s_CFLAGS=$(CFLAGS) $(CFLAGS_DYNLIB) %s %s %s\n",
                project->projectName,
                gccDefines,
                gccIncludes,
                cStandardVersion);

        fprintf(makefile, "%s_LDFLAGS=$(LDFLAGS) $(LDFLAGS_DYNLIB)", project->projectName);
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

        fprintf(makefile, "%s_LDFLAGS=$(LDFLAGS)", project->projectName);
        if (project->linkerScript != NULL) {
            fprintf(makefile, " -T%s\n\n", project->linkerScript);
        } else {
            fprintf(makefile, "\n\n");
        }
    }
    free(gccIncludes);
    free(gccDefines);

    UtilStringArray objectFiles = UtilCreateStringArray();
    UtilStringArray dependencyFiles = UtilCreateStringArray();

    for (size_t i = 0; i < project->sources.stringCount; i++) {
        char *sourceFile = project->sources.data[i];

        const size_t sourceFileLength = strlen(basename(sourceFile));
        char *objectFile = malloc(sourceFileLength + 1);
        strncpy(objectFile, basename(sourceFile), sourceFileLength);
        char *extPointer = strrchr(objectFile, '.');
        assert(extPointer != NULL);
        *(extPointer) = '\0';

        char *dependencyFile = malloc(sourceFileLength + 1);
        strncpy(dependencyFile, basename(sourceFile), sourceFileLength);
        extPointer = strrchr(dependencyFile, '.');
        assert(extPointer != NULL);
        *(extPointer) = '\0';

        const size_t objectPathLength = strlen("bin/") +
                                        strlen(project->projectName) +
                                        strlen(objectFile) +
                                        4;
        char *objectPath = malloc(objectPathLength + 1);
        snprintf(objectPath,
                 objectPathLength,
                 "bin/%s/%s.o",
                 project->projectName,
                 objectFile);

        const size_t dependencyPathLength = strlen("bin/") +
                                            strlen(project->projectName) +
                                            strlen(dependencyFile) +
                                            4;
        char *dependencyPath = malloc(dependencyPathLength + 1);
        snprintf(dependencyPath,
                 dependencyPathLength,
                 "bin/%s/%s.d",
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
        UtilAppendToStringArray(&objectFiles, objectPath);
        UtilAppendToStringArray(&dependencyFiles, dependencyPath);
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

    if (project->type == BLD_EXECUTABLE) {
        fprintf(makefile,
                PLAT_EXE_TEMPLATE ": $(%s_OBJ)\n",
                project->projectName,
                project->projectName);
        fprintf(makefile,
                "\t$(CC) $(%s_LDFLAGS) $(%s_OBJ) -o bin/" PLAT_EXE_TEMPLATE "\n\n",
                project->projectName,
                project->projectName,
                project->projectName);
    } else if (project->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile,
                PLAT_DYNLIB_TEMPLATE ": $(%s_OBJ)\n",
                project->projectName,
                project->projectName);
        fprintf(makefile,
                "\t$(CC) -shared $(%s_LDFLAGS) $(%s_OBJ) -o bin/" PLAT_DYNLIB_TEMPLATE "\n\n",
                project->projectName,
                project->projectName,
                project->projectName);
    } else if (project-> type == BLD_STATIC_LIBRARY) {
        fprintf(makefile,
                PLAT_STATICLIB_TEMPLATE ": $(%s_OBJ)\n",
                project->projectName,
                project->projectName);
        fprintf(makefile,
                "\t$(AR) rcs bin/" PLAT_STATICLIB_TEMPLATE " $(%s_OBJ)\n\n",
                project->projectName,
                project->projectName);
    } else {
        assert(false);
    }

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
        fprintf(makefile, "all: " PLAT_EXE_TEMPLATE "\n\n", defaultTarget->projectName);
    } else if (defaultTarget->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile, "all: " PLAT_DYNLIB_TEMPLATE "\n\n", defaultTarget->projectName);
    } else if (defaultTarget->type == BLD_STATIC_LIBRARY) {
        fprintf(makefile, "all: " PLAT_STATICLIB_TEMPLATE "\n\n", defaultTarget->projectName);
    }

    fprintf(makefile, "CFLAGS=-Wall -Wextra -MMD\n");
    fprintf(makefile, "CFLAGS_DYNLIB=-fPIC\n");
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
            fprintf(makefile, "TO_CLEAN+=" PLAT_EXE_TEMPLATE "\n\n", project->projectName);
        } else if (project->type == BLD_DYNAMIC_LIBRARY) {
            fprintf(makefile, "TO_CLEAN+=" PLAT_DYNLIB_TEMPLATE "\n\n", project->projectName);
        } else if (project->type == BLD_STATIC_LIBRARY) {
            fprintf(makefile, "TO_CLEAN+=" PLAT_STATICLIB_TEMPLATE "\n\n", project->projectName);
        }
    }

    fprintf(makefile, "-include bin/*.d\n");

    fprintf(makefile, "clean:\n");
    fprintf(makefile, "\trm -f $(TO_CLEAN)\n");
}
