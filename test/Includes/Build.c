#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *exe = BldNewProject("Test", BLD_EXECUTABLE, BLD_LANGUAGE_DEFAULT);
    BldAddSources(exe, "Test.c");
    BldAddIncludePaths(exe, "includes");

    BldGenerate(exe);
}
