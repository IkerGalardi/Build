#include "Generators.h"
#include "Platform.h"

#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>

#include "Project.h"

static char *PBldDefinesToGccStyle(char *defines)
{
    char *result = NULL;
    size_t resultLength = 0;

    if (defines == NULL) {
        result = malloc(1);
        *result = '\0';
        return result;
    }

    char *token = strtok(defines, " ");
    while (token != NULL) {
        size_t tokenLength = strlen(token);
        result = realloc(result, resultLength + 2 + tokenLength + 1);
        resultLength = resultLength + 2 + tokenLength;

        strncat(result, "-D", resultLength);
        strncat(result, token, resultLength);
        strncat(result, " ", resultLength);

        token = strtok(NULL, " ");
    }

    return result;
}

void PBldAddProjectToMakefile(FILE *makefile, BldProject *project)
{
    // TODO: too many allocations :S
    assert(makefile != NULL);
    assert(project != NULL);

    char *gccDefines = PBldDefinesToGccStyle(project->defines);
    if (project->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile, "%s_CFLAGS=$(CFLAGS) $(CFLAGS_DYNLIB) %s\n",
                project->projectName,
                gccDefines);
        fprintf(makefile, "%s_LDFLAGS=$(LDFLAGS) $(LDFLAGS_DYNLIB)\n\n", project->projectName);
    } else {
        fprintf(makefile, "%s_CFLAGS=$(CFLAGS) %s\n", project->projectName, gccDefines);
        fprintf(makefile, "%s_LDFLAGS=$(LDFLAGS)\n\n", project->projectName);
    }
    free(gccDefines);

    char *sourcesCopy = malloc(strlen(project->sources) + 1);
    strcpy(sourcesCopy, project->sources);

    char *objNames = NULL;
    size_t objNamesLen = 0;
    char *token = strtok(sourcesCopy, " ");
    while(token != NULL) {
        size_t basenameLen = strlen(token);
        char *objName = malloc(basenameLen + 1);
        char *basenameStr = basename(token);
        strncpy(objName, basenameStr, basenameLen + 1);

        // TODO: assumes that there is a dot character and at least another character
        //       after it. Don't assume that you idiot.
        char *extPointer = strrchr(objName, '.');
        assert(extPointer != NULL);
        *(extPointer+1) = 'o';
        *(extPointer+2) = '\0';

        size_t objNameLen = strlen(objName);
        objNames = realloc(objNames, objNamesLen + strlen(objName) + 6);
        strncat(objNames, "bin/", objNamesLen + strlen(objName) + 5);
        strncat(objNames, objName, objNamesLen + strlen(objName) + 5);
        strncat(objNames, " ", objNamesLen + strlen(objName) + 5);
        objNamesLen += objNameLen;

        fprintf(makefile, "bin/%s: %s\n", objName, token);
        fprintf(makefile, "\t$(CC) -c $(%s_CFLAGS) -o bin/%s %s\n\n", project->projectName, objName, token);

        token = strtok(NULL, " ");
        free(objName);
    }

    free(sourcesCopy);

    fprintf(makefile, "%s_OBJ = %s\n\n", project->projectName, objNames);
    if (project->type == BLD_EXECUTABLE) {
        fprintf(makefile,
                PLAT_EXE_TEMPLATE ": $(%s_OBJ)\n",
                project->projectName,
                project->projectName);
        fprintf(makefile,
                "\t$(CC) $(%s_LDFLAGS) $(%s_OBJ) -o " PLAT_EXE_TEMPLATE "\n\n",
                project->projectName,
                project->projectName,
                project->projectName);
    } else if (project->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile,
                PLAT_DYNLIB_TEMPLATE ": $(%s_OBJ)\n",
                project->projectName,
                project->projectName);
        fprintf(makefile,
                "\t$(CC) -shared $(%s_LDFLAGS) $(%s_OBJ) -o " PLAT_DYNLIB_TEMPLATE "\n\n",
                project->projectName,
                project->projectName,
                project->projectName);
    } else if (project-> type == BLD_STATIC_LIBRARY) {
        fprintf(makefile,
                PLAT_STATICLIB_TEMPLATE ": $(%s_OBJ)\n",
                project->projectName,
                project->projectName);
        fprintf(makefile,
                "\t$(AR) rcs " PLAT_STATICLIB_TEMPLATE " $(%s_OBJ)\n\n",
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

    fprintf(makefile, "CFLAGS=-Wall -Wextra\n");
    fprintf(makefile, "CFLAGS_DYNLIB=-fPIC\n");
    fprintf(makefile, "LDFLAGS_DYNLIB=-fPIC\n\n");

    fprintf(makefile, "ALL_OBJ=\n\n");

    // Add a way to rebuild the makefile if the build script has been changed.
    fprintf(makefile, "Makefile: Build.c\n");
    fprintf(makefile, "\t$(CC) -o Build Build.c -lBuild\n");
    fprintf(makefile, "\t./Build\n\n");

    for (size_t i = 0; i < projectCount; i++) {
        PBldAddProjectToMakefile(makefile, &projects[i]);

        fprintf(makefile, "ALL_OBJ+=$(%s_OBJ)\n\n", projects->projectName);
    }

    fprintf(makefile, "clean:\n");
    fprintf(makefile, "\trm -f $(ALL_OBJ)\n");
}
