/*!  
 *@brief    纹理
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include <vector>
#include <string>
#include "TRSExport.h"

//纹理数据，表征一个纹理的数据结构
struct TextureData
{
    unsigned int id;
    std::string strImageFile;
    std::string strSampleName;
    TextureData()
    {

    }
    TextureData(unsigned int nID, std::string strImage, std::string strName)
        :id(nID), strImageFile(strImage), strSampleName(strName)
    {

    }
};

class TRS_EXPORT TRSTexture
{
public:
    TRSTexture();
    TRSTexture(int texCount, ...);
    ~TRSTexture();

    int createTexture(const std::string& imageFile, const std::string& sampleName = "");

    void activeAllTextures(unsigned int program);

    //当多个纹理使用相同的纹理图片，可以共享
    void addSharedTexture(const TextureData& textureData);

    //查询纹理对象中是否存在文件名为imageFile的纹理，如果有，返回纹理数据
    bool getTextureDataByName(const std::string& imageFile, TextureData& outData);

    int count();

    std::string debugInfo();

protected:
    std::vector<unsigned int> m_nTextures;      //纹理ID
    std::vector<std::string> m_sImageFileNames; //纹理图片的路径
    std::vector<std::string> m_sSampleNames;    //着色器中采样器的名称
};

