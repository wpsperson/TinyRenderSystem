/*!  
 *@brief    纹理
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include <vector>
#include "TRSExport.h"

class TRS_EXPORT TRSTexture
{
public:
    TRSTexture();
    TRSTexture(int texCount, ...);
    ~TRSTexture();

    void createTexture(const std::string& imageFile);

    void activeTexutres(std::vector<unsigned int> idxs);

    void activeAllTextures();

    int count();

protected:
    std::vector<unsigned int> m_nTextures;

};

