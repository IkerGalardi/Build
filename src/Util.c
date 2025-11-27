#include "Util.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

PBldStringArray PBldCreateStringArray()
{
    PBldStringArray sa;
    sa.stringCount = 0;
    sa.data = NULL;

    return sa;
}

void PBldAppendToStringArray(PBldStringArray *sa, char *string)
{
    sa->stringCount++;
    sa->data = realloc(sa->data, sizeof(char*) * sa->stringCount);
    assert(sa->data != NULL);

    size_t stringLenght = strlen(string);
    char *newString = malloc(stringLenght + 1);
    strncpy(newString, string, stringLenght);
    newString[stringLenght] = '\0';
    assert(strcmp(newString, string) == 0);

    sa->data[sa->stringCount - 1] = newString;
}
