#include "../include/Build.h"

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Project.h"
#include "Generators.h"

BldProject projects[100];
size_t projectIndex = 0;

BldProject *BldNewProject(char *projectName,
                          BldProjectType type,
                          BldLanguage language)
{
    assert(projectIndex < 100);
    BldProject *project = &projects[projectIndex];
    projectIndex++;

    memset(project->projectName, 0, 64);
    strncpy(project->projectName, projectName, 63);
    project->type = type;
    project->sources = NULL;
    project->language = language;

    return project;
}

void PBldAddSources(BldProject *project,
                    ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *source = va_arg(ap, char *);
    while (source != NULL) {
        size_t sourceLength = strlen(source);
        size_t projectLength = 0;
        if (project->sources != NULL)
            projectLength = strlen(project->sources);

        project->sources = realloc(project->sources, sourceLength + projectLength + 2);
        strncat(project->sources, source, sourceLength + projectLength + 1);
        strncat(project->sources, " ", sourceLength + projectLength + 1);

        source = va_arg(ap, char *);
    }
    va_end(ap);
}

void PBldAddIncludePaths(BldProject *project,
                         ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *includePath = va_arg(ap, char *);
    while (includePath != NULL) {
        size_t includePathLength = strlen(includePath);
        size_t projectLength = 0;
        if (project->includePaths != NULL)
            projectLength = strlen(project->includePaths);

        project->includePaths = realloc(project->includePaths,
                                        includePathLength + projectLength + 2);
        strncat(project->includePaths,
                includePath,
                includePathLength + projectLength + 1);
        strncat(project->includePaths, " ", includePathLength + projectLength + 1);

        includePath = va_arg(ap, char *);
    }
    va_end(ap);
}

void PBldAddDefines(BldProject *project, ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *define = va_arg(ap, char *);
    while (define != NULL) {
        size_t defineLength = strlen(define);
        size_t projectDefineLength = 0;
        if (project->defines != NULL) {
            projectDefineLength = strlen(project->defines);
        }

        project->defines = realloc(project->defines,
                                   defineLength + projectDefineLength + 2);

        strncat(project->defines, define, defineLength + projectDefineLength + 1);
        strncat(project->defines, " ", defineLength + projectDefineLength + 1);

        define = va_arg(ap, char *);
    }

    va_end(ap);
}

void BldGenerate(BldProject *defaultTarget)
{
    assert(defaultTarget != NULL);

    FILE *makefile = fopen("Makefile", "w");
    assert(makefile != NULL);

    PBldGenerateMakefile(makefile, defaultTarget, projects, projectIndex);
}
