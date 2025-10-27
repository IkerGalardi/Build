#include <stdio.h>

int main()
{
    #ifdef __STDC_VERSION__
    if (__STDC_VERSION__ == 202311L) {
        return 0;
    } else {
        return 1;
    }
    #else
    return 1;
    #endif
}
