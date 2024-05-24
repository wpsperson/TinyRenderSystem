#pragma once

class TRSGroup;

extern "C"
{
    __declspec(dllexport) int testFunction(int first, int second);

    __declspec(dllexport) TRSGroup *loadStepFile(const char *StepFile);
}

