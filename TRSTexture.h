/*!  
 *@brief    ����
 *@author   wangps
 *@date     2019��5��3��
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

