#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *exe = BldNewProject("TestExe", BLD_EXECUTABLE, BLD_LANGUAGE_DEFAULT);
    BldSetSources(exe, "TestExe.c");

    BldProject *dlib = BldNewProject("TestDLib", BLD_DYNAMIC_LIBRARY, BLD_LANGUAGE_DEFAULT);
    BldSetSources(dlib, "TestDLib.c");

    BldProject *slib = BldNewProject("TestSLib", BLD_STATIC_LIBRARY, BLD_LANGUAGE_DEFAULT);
    BldSetSources(slib, "TestSLib.c");

    BldGenerate(exe);
}
