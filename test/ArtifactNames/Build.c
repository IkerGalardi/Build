#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *exe = BldNewProject("TestExe", BLD_EXECUTABLE, BLD_LANGUAGE_DEFAULT);
    BldAddSources(exe, "TestExe.c");

    BldProject *dlib = BldNewProject("TestDLib", BLD_DYNAMIC_LIBRARY, BLD_LANGUAGE_DEFAULT);
    BldAddSources(dlib, "TestDLib.c");

    BldProject *slib = BldNewProject("TestSLib", BLD_STATIC_LIBRARY, BLD_LANGUAGE_DEFAULT);
    BldAddSources(slib, "TestSLib.c");

    BldGenerate(exe);
}
