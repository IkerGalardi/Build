#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *exe = BldNewProject("Test", BLD_EXECUTABLE, BLD_LANGUAGE_DEFAULT);
    BldSetSources(exe, "Test.c");
    BldSetIncludePaths(exe, "includes");

    BldGenerate(exe);
}
