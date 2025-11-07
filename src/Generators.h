#ifndef GENERATORS_H
#define GENERATORS_H

#include <stddef.h>
#include <stdio.h>

#include "Build.h"

void PBldGenerateMakefile(FILE *makefile,
                          BldProject *defaultTarget,
                          BldProject *projects,
                          size_t projectCount);

#endif // GENERATORS_H
