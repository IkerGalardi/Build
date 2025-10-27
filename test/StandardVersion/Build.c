#include "Build.h"

static BldProject *SingleFileExecutable(char *name, char *file, BldLanguage lang)
{
    BldProject *exe = BldNewProject(name, BLD_EXECUTABLE, lang);
    BldAddSources(exe, file);
    return exe;
}

int main()
{
    BldProject *c90 = SingleFileExecutable("c90", "90.c", BLD_LANGUAGE_C90);
    BldProject *c99 = SingleFileExecutable("c99", "99.c", BLD_LANGUAGE_C99);
    BldProject *c11 = SingleFileExecutable("c11", "11.c", BLD_LANGUAGE_C11);
    BldProject *c17 = SingleFileExecutable("c17", "17.c", BLD_LANGUAGE_C17);
    BldProject *c23 = SingleFileExecutable("c23", "23.c", BLD_LANGUAGE_C23);

    BldGenerate(c90);
}
