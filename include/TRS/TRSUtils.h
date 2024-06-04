#pragma once

#include "TRS/TRSExport.h"

//deprecated
TRS_EXPORT char* getTextFromFile(const char* fileName);

//recommend
TRS_EXPORT char* readTextContent(const char* strFileName);
