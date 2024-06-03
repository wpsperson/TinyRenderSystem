#pragma once
#pragma warning( disable: 4251 )

#ifdef TRSLIB
    #define TRS_EXPORT __declspec(dllexport)
#else
    #define TRS_EXPORT __declspec(dllimport)
#endif


