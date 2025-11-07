#include "Build.h"

int main(int argc, char **argv)
{
    BldProject *exeEmpty = BldNewProject("DefineEmpty",
                                         BLD_EXECUTABLE,
                                         BLD_LANGUAGE_DEFAULT);
    BldSetSources(exeEmpty, "TestEmpty.c");
    BldSetDefines(exeEmpty, "DEFINE_EMPTY");

    BldProject *exeNumber = BldNewProject("DefineNumber",
                                          BLD_EXECUTABLE,
                                          BLD_LANGUAGE_DEFAULT);
    BldSetSources(exeNumber, "TestNumber.c");
    BldSetDefines(exeNumber, "DEFINE_NUMBER=7");

    BldProject *exeString = BldNewProject("DefineString",
                                          BLD_EXECUTABLE,
                                          BLD_LANGUAGE_DEFAULT);
    BldSetSources(exeString, "TestString.c");
    BldSetDefines(exeString, "DEFINE_STRING=\"STRING\"");

    BldGenerate(exeEmpty);
}
