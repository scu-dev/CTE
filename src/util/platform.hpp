#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__NT__)
    #define _CTE_WINDOWS 1
    #include <windows.h>
#else
    #define _CTE_UNIX 1
    #include <unistd.h>
#endif