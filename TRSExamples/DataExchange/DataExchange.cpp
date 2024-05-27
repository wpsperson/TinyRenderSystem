#include "DataExchange.h"
#include "TRS/TRSNode.h"
#include "StepConverter.h"

int testFunction(int first, int second)
{
    return first * 10 + second;
}

TRSNode* loadStepFile(const char* StepFile)
{
    StepConverter converter;
    return converter.readStepFile(StepFile);
}

