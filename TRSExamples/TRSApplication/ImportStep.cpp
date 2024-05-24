#include "ImportStep.h"

#include <Windows.h>
#include <iostream>

const char* strDllFile = "DataExchanged.dll";
const char* strTestFunction = "testFunction";

typedef int (* FuncType)(int, int);

ImportStep::ImportStep()
{
}

ImportStep::~ImportStep()
{
}

void ImportStep::readStepFile(const char* file_name)
{
    HINSTANCE instance = LoadLibrary(strDllFile);
    if (!instance) {
        DWORD error = GetLastError();
        std::cout << "error code: " << error << std::endl;
        return ;
    }
    // strTestFunction
        // resolve function address here
    FuncType function = (FuncType)GetProcAddress(instance, strTestFunction);
    if (!function) {
        DWORD error = GetLastError();
        std::cout << "error code: " << error << std::endl;
        return ;
    }

    int result = function(12, 8);
    std::cout << "result : " << result << std::endl;

}
