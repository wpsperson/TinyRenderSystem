#pragma once

#include <string>
#include "TRS/TRSNode.h"

class TRSNode;

class ImportStep
{
public:
    static TRSNode* readStepFile(const char* file_name, std::string &error);
    static TRSNode* readSTLFile(const char* file_name, std::string& error);
};
