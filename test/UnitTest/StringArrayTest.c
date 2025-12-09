#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "Util.h"

int main()
{
    PBldStringArray sa = PBldCreateStringArray();

    PBldAppendToStringArray(&sa, "First");
    PBldAppendToStringArray(&sa, "Second");
    PBldAppendToStringArray(&sa, "Third");

    bool condition = strcmp(sa.data[0], "First") == 0 &&
                     strcmp(sa.data[1], "Second") == 0 &&
                     strcmp(sa.data[2], "Third") == 0;
    if (condition == true) {
        printf(" · StringArray append:  PASS\n");
    } else {
        printf(" · StringArray append:  FAIL\n");
    }
}
