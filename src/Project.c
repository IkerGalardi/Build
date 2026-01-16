#include "../include/Build.h"

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <libgen.h>

#include "Project.h"
#include "Generators.h"
#include "Util.h"

BldProject projects[100];
size_t projectIndex = 0;

void PBldAppendDependencyHeaderDirectories(PBldProjectArray projects,
                                           PBldStringArray *includes)
{
    for (size_t i = 0; i < projects.projectCount; i++) {
        BldProject *project = &projects.data[i];

        for (size_t j = 0; j < project->publicHeaders.stringCount; j++) {
            char *headerDir = dirname(project->publicHeaders.data[j]);
            PBldAppendToStringArray(includes, headerDir);
        }
    }
}

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
    project->sources = PBldCreateStringArray();
    project->includePaths = PBldCreateStringArray();
    project->defines = PBldCreateStringArray();
    project->dependencies = PBldCreateProjectArray();
    project->publicHeaders = PBldCreateStringArray();
    project->language = language;
    project->linkerScript = NULL;

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
        FILE *sourceFile = fopen(source, "r");
        if (sourceFile == NULL) {
            fprintf(stderr, "Build: source file %s does not exist\n", source);
            exit(1);
        }

        PBldAppendToStringArray(&project->sources, source);
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
        PBldAppendToStringArray(&project->includePaths, includePath);
        includePath = va_arg(ap, char *);
    }
    va_end(ap);
}

void BldAddIncludePathArray(BldProject *project, char **includes, size_t nIncludes)
{
    for (size_t i = 0; i < nIncludes; i++) {
        PBldAppendToStringArray(&project->includePaths, includes[i]);
    }
}

void PBldAddDefines(BldProject *project, ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *define = va_arg(ap, char *);
    while (define != NULL) {
        PBldAppendToStringArray(&project->defines, define);
        define = va_arg(ap, char *);
    }

    va_end(ap);
}

void BldAddDefinesArray(BldProject *project, char **defines, size_t nDefines)
{
    for (size_t i = 0; i < nDefines; i++) {
        PBldAppendToStringArray(&project->defines, defines[i]);
    }
}

void PBldAddDependencies(BldProject *project, ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    BldProject *dependency = va_arg(ap, BldProject *);
    while (dependency != NULL) {
        PBldAppendToProjectArray(&project->dependencies, dependency);
        dependency = va_arg(ap, BldProject *);
    }

    va_end(ap);
}

void BldAddDependencyArray(BldProject *project,
                           BldProject **dependencies, size_t nDependencies)
{
    for (size_t i = 0; i < nDependencies; i++) {
        PBldAppendToProjectArray(&project->dependencies, dependencies[i]);
    }
}

void PBldAddPublicHeader(BldProject *project, ...)
{
    va_list ap;
    va_start(ap, project);

    assert(project != NULL);

    char *publicHeader = va_arg(ap, char *);
    while (publicHeader != NULL) {
        PBldAppendToStringArray(&project->publicHeaders, publicHeader);
        publicHeader = va_arg(ap, char *);
    }

    va_end(ap);
}

void BldAddPublicHeaderArray(BldProject *project, char **publicHeaders, size_t nPublicHeaders)
{
    for (size_t i = 0; i < nPublicHeaders; i++) {
        PBldAppendToStringArray(&project->publicHeaders, publicHeaders[i]);
    }
}

void BldSetLinkerScript(BldProject *project, char *linkerScript)
{
    project->linkerScript = linkerScript;
}

void BldGenerate(BldProject *defaultTarget)
{
    assert(defaultTarget != NULL);

    FILE *makefile = fopen("Makefile", "w");
    assert(makefile != NULL);

    for (size_t i = 0; i < projectIndex; i++) {
        BldProject *project = &projects[i];

        PBldAppendDependencyHeaderDirectories(project->dependencies,
                                              &project->includePaths);
    }

    PBldGenerateMakefile(makefile, defaultTarget, projects, projectIndex);

    // Why not simply call the mkdir system call? Don't want to bother with umasks for now.
    system("mkdir -p bin/");

    for (size_t i = 0; i < projectIndex; i++) {
        char commandBuffer[255] = {0};
        snprintf(commandBuffer, 254, "mkdir -p bin/%s.d", projects[i].projectName);
        system(commandBuffer);
    }
}
