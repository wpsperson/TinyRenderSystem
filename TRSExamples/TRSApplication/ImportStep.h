#pragma once

class ImportStep
{
public:
    ImportStep();
    ~ImportStep();
    void readStepFile(const char* file_name);
};
