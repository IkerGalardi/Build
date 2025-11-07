#include "../include/Build.h"

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Project.h"
#include "Generators.h"
#include "Util.h"

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
    project->sources = UtilCreateStringArray();
    project->includePaths = UtilCreateStringArray();
    project->defines = UtilCreateStringArray();
    project->language = language;

    return project;
}

void PBldSetSources(BldProject *project,
                    ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *source = va_arg(ap, char *);
    while (source != NULL) {
        UtilAppendToStringArray(&project->sources, source);
        source = va_arg(ap, char *);
    }
    va_end(ap);
}

void PBldSetIncludePaths(BldProject *project,
                         ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *includePath = va_arg(ap, char *);
    while (includePath != NULL) {
        UtilAppendToStringArray(&project->includePaths, includePath);
        includePath = va_arg(ap, char *);
    }
    va_end(ap);
}

void PBldSetDefines(BldProject *project, ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *define = va_arg(ap, char *);
    while (define != NULL) {
        UtilAppendToStringArray(&project->defines, define);
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

    // Why not simply call the mkdir system call? Don't want to bother with umasks for now.
    system("mkdir -p bin/");
}
