/*!  
 *@brief    纹理
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include <vector>
#include <string>
#include "TRSExport.h"

class TRS_EXPORT TRSTexture
{
public:
    TRSTexture();
    TRSTexture(int texCount, ...);
    ~TRSTexture();

    void createTexture(const std::string& imageFile, const std::string& sampleName = "");

    void activeAllTextures();

    int count();

    std::vector<std::string> getSampleNames();

protected:
    std::vector<unsigned int> m_nTextures;
    std::vector<std::string> m_sSampleNames;
};

