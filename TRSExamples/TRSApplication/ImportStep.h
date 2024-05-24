#pragma once

#include <string>
class TRSGroup;

class ImportStep
{
public:
    static TRSGroup *readStepFile(const char* file_name, std::string &error);
};
