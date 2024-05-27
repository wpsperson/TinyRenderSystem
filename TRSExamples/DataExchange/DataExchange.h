#pragma once

class TRSNode;

extern "C"
{
    __declspec(dllexport) int testFunction(int first, int second);

    __declspec(dllexport) TRSNode*loadStepFile(const char *StepFile);
}

