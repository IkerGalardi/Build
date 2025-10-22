#include <stdio.h>
#include <string.h>

int main()
{
    #ifdef DEFINE_STRING
    if (strcmp(DEFINE_STRING, "STRING") == 0) {
    printf(" · STRING correcly defined: PASS\n");
    } else {
    printf(" · STRING correcly defined: FAIL\n");
    }
    #else
    printf(" · STRING correcly defined: FAIL\n");
    #endif // STRING
}
