#include "DataExchange.h"
#include "TRS/TRSGroup.h"
#include "StepConverter.h"

int testFunction(int first, int second)
{
    return first * 10 + second;
}

TRSGroup* loadStepFile(const char* StepFile)
{
    TRSGroup* group = new TRSGroup;
    StepConverter converter;
    bool success = converter.readSTEP(std::string(StepFile), group);
    if (!success)
    {
        delete group;
        return nullptr;
    }
    return group;
}

