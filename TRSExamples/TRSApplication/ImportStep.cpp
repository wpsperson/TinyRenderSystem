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
typedef TRSNode* (*LoadFuncType)(const char*);


TRSNode* ImportStep::readStepFile(const char* file_name, std::string& error)
{
    HINSTANCE instance = LoadLibrary(strDllFile);
    if (!instance) {
        DWORD code = GetLastError();
        error = "Fail to LoadLibrary, error code: " + std::to_string(code);
        return nullptr;
    }
    LoadFuncType function = (LoadFuncType)GetProcAddress(instance, strLoadStepFile);
    if (!function) {
        DWORD code = GetLastError();
        error = "Fail to GetProcAddress, error code: " + std::to_string(code);
        return nullptr;
    }

    TRSNode* result = function(file_name);
    return result;
}
