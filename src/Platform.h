#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(__APPLE__)
    #define PLAT_EXE_TEMPLATE "%s"
    #define PLAT_DYNLIB_TEMPLATE "lib%s.dylib"
    #define PLAT_STATICLIB_TEMPLATE "lib%s.a"

    #define PLAT_IS_APPLE true
    #define PLAT_IS_UNIX  false
#elif defined(__unix__)
    #define PLAT_EXE_TEMPLATE "%s"
    #define PLAT_DYNLIB_TEMPLATE "lib%s.so"
    #define PLAT_STATICLIB_TEMPLATE "lib%s.a"

    #define PLAT_IS_APPLE false
    #define PLAT_IS_UNIX  true
#else
    #error "Unknown platform, not supported"
#endif

#endif // PLATFORM_H
