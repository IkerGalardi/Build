#include "include/Build.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    BldProject *lib = BldNewProject("Build",
                                    BLD_DYNAMIC_LIBRARY,
                                    BLD_LANGUAGE_C17);
    BldAddSources(lib, "src/Project.c", "src/MakefileGenerator.c", "src/Util.c");
    BldAddIncludePaths(lib, "include");

    BldGenerate(lib);
}
