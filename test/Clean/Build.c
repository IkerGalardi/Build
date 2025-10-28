#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *lib = BldNewProject("meth", BLD_DYNAMIC_LIBRARY, BLD_LANGUAGE_DEFAULT);
    BldAddSources(lib, "a.c");

    BldGenerate(lib);
}
