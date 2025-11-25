#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *lib = BldNewProject("lib", BLD_STATIC_LIBRARY, BLD_LANGUAGE_DEFAULT);
    BldAddSources(lib, "lib.c");

    BldProject *exe = BldNewProject("exe", BLD_EXECUTABLE, BLD_LANGUAGE_DEFAULT);
    BldAddSources(exe, "exe.c");
    BldAddDependencies(exe, lib);

    BldGenerate(exe);
}
