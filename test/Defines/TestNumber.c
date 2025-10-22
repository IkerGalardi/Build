#include <stdio.h>

int main()
{
    #ifdef DEFINE_NUMBER
    if (DEFINE_NUMBER == 7) {
        printf(" · DEFINE_NUMBER correctly defined: PASS\n");
        return 0;
    } else {
        printf(" · DEFINE_NUMBER correctly defined: FAIL\n");
        return 1;
    }
    #else
    printf(" · DEFINE_NUMBER correctly defined: FAIL\n");
    return 1;
    #endif // NUMBER
}
