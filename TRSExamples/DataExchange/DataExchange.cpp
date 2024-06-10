#include "DataExchange.h"
#include "TRS/TRSNode.h"
#include "TRS/TRSGroup.h"
#include "StepConverter.h"
#include "AssimpLoader.h"

int testFunction(int first, int second)
{
    return first * 10 + second;
}

TRSNode* loadStepFile(const char* StepFile)
{
    StepConverter converter;
    return converter.readStepFile(StepFile);
}

TRSNode* loadSTLFile(const char* stlFile)
{
    AssimpLoader assimpLoader;
    std::string fileName(stlFile);
    return assimpLoader.loadByAssimp(fileName);
}

