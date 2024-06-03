#pragma once

#include <vector>
#include <string>

#include "TRS/TRSExport.h"


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

    void addSharedTexture(const TextureData& textureData);

    // query specified file, if eixst, return the outData 
    bool getTextureDataByName(const std::string& imageFile, TextureData& outData);

    int count();

    std::string debugInfo();

protected:
    std::vector<unsigned int> m_nTextures;      // texture ID
    std::vector<std::string> m_sImageFileNames; // texture images
    std::vector<std::string> m_sSampleNames;    // sample names
};

