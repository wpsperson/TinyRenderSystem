/*!  
 *@brief    ���߷����͹�����
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#include <iostream>
#include <string>
#include "TRSExport.h"

//deprecated
TRS_EXPORT char* getTextFromFile(const char* fileName);

//recommend
TRS_EXPORT char* readTextFile(const std::string& strFileName);

