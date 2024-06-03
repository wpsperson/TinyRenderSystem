#pragma once

#include <iostream>
#include <string>

#include "TRS/TRSExport.h"

static double TRS_PI = 3.141592653589793238;

//deprecated
TRS_EXPORT char* getTextFromFile(const char* fileName);

//recommend
TRS_EXPORT char* readTextFile(const std::string& strFileName);
