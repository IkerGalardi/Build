#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *exeEmpty = BldNewProject("DefineEmpty",
                                         BLD_EXECUTABLE,
                                         BLD_LANGUAGE_DEFAULT);
    BldAddSources(exeEmpty, "TestEmpty.c");
    BldAddDefines(exeEmpty, "DEFINE_EMPTY");

    BldProject *exeNumber = BldNewProject("DefineNumber",
                                          BLD_EXECUTABLE,
                                          BLD_LANGUAGE_DEFAULT);
    BldAddSources(exeNumber, "TestNumber.c");
    BldAddDefines(exeNumber, "DEFINE_NUMBER=7");

    BldProject *exeString = BldNewProject("DefineString",
                                          BLD_EXECUTABLE,
                                          BLD_LANGUAGE_DEFAULT);
    BldAddSources(exeString, "TestString.c");
    BldAddDefines(exeString, "DEFINE_STRING=\"STRING\"");

    BldGenerate(exeEmpty);
}
