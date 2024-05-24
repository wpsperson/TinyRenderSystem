#include "ImportStep.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include <iostream>

const char* strDllFile = "DataExchanged.dll";
const char* strTestFunction = "testFunction";
const char* strLoadStepFile = "loadStepFile";

typedef int (* FuncType)(int, int);
typedef TRSGroup* (*LoadFuncType)(const char*);


TRSGroup* ImportStep::readStepFile(const char* file_name, std::string& error)
{
    HINSTANCE instance = LoadLibrary(strDllFile);
    if (!instance) {
        DWORD code = GetLastError();
        error = "Fail to LoadLibrary, error code: " + std::to_string(code);
        return nullptr;
    }
    LoadFuncType function = (LoadFuncType)GetProcAddress(instance, strTestFunction);
    if (!function) {
        DWORD code = GetLastError();
        error = "Fail to GetProcAddress, error code: " + std::to_string(code);
        return nullptr;
    }

    TRSGroup* result = function(file_name);
    return result;
}
