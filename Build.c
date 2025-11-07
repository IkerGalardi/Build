#include "include/Build.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    BldProject *lib = BldNewProject("Build",
                                    BLD_DYNAMIC_LIBRARY,
                                    BLD_LANGUAGE_C17);
    BldSetSources(lib, "src/Project.c", "src/MakefileGenerator.c", "src/Util.c");
    BldSetIncludePaths(lib, "include");

    BldGenerate(lib);
}
