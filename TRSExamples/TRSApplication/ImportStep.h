#pragma once

#include <string>
class TRSNode;

class ImportStep
{
public:
    static TRSNode* readStepFile(const char* file_name, std::string &error);
    static TRSNode* readSTLFile(const char* file_name, std::string& error);
};
