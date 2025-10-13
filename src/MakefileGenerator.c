#include "Generators.h"

#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>

void PBldAddProjectToMakefile(FILE *makefile, BldProject *project)
{
    // TODO: too many allocations :S

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
        fprintf(makefile, "\t$(CC) -c $(CFLAGS)-o bin/%s %s\n\n", objName, token);

        token = strtok(NULL, " ");
        free(objName);
    }

    free(sourcesCopy);

    fprintf(makefile, "%s_OBJ = %s\n\n", project->projectName, objNames);
    if (project->type == BLD_EXECUTABLE) {
        fprintf(makefile, "%s: $(%s_OBJ)\n", project->projectName, project->projectName);
    } else if (project->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile, "lib%s.so: $(%s_OBJ)\n", project->projectName, project->projectName);
    } else {
        fprintf(makefile, "lib%s.a: $(%s_OBJ)\n", project->projectName, project->projectName);
    }
    fprintf(makefile, "\t$(CC) $(LDFLAGS) $(%s_OBJ) -o %s\n\n", project->projectName, project->projectName);

}

void PBldGenerateMakefile(FILE *makefile,
                          BldProject *defaultTarget,
                          BldProject *projects,
                          size_t projectCount)
{
    if (defaultTarget->type == BLD_EXECUTABLE) {
        fprintf(makefile, "all: %s\n\n", defaultTarget->projectName);
    } else if (defaultTarget->type == BLD_DYNAMIC_LIBRARY) {
        fprintf(makefile, "all: lib%s.so\n\n", defaultTarget->projectName);
    } else if (defaultTarget->type == BLD_STATIC_LIBRARY) {
        fprintf(makefile, "all: lib%s.a\n\n", defaultTarget->projectName);
    }

    // Add a way to rebuild the makefile if the build script has been changed.
    fprintf(makefile, "Makefile: Build.c\n");
    fprintf(makefile, "\t$(CC) -o Build Build.c -lBuild\n");
    fprintf(makefile, "\t./Build\n\n");

    for (int i = 0; i < projectCount; i++) {
        PBldAddProjectToMakefile(makefile, &projects[i]);
    }
}
