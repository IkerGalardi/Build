#include <stdio.h>

int main()
{
    #ifdef DEFINE_EMPTY
    printf(" · DEFINE_EMPTY defined:            PASS\n");
    return 0;
    #else
    printf(" · DEFINE_EMPTY defined:            FAIL\n");
    return 1;
    #endif // DEFINE_DEFINED
}
