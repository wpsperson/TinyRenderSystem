/*!  
 *@brief    ����
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#pragma warning( disable: 4251 )
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

protected:
    std::vector<unsigned int> textures;

};

