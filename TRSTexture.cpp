#include "TRSTexture.h"
#include <iostream>
#include <cstdarg>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TRSConst.h"


using namespace std;

TRSTexture::TRSTexture()
{

}

TRSTexture::TRSTexture(int texCount, ...)
{
    va_list ap;
    va_start(ap, texCount);
    std::string imageFile;
    for (int i = 0; i < texCount; i++)
    {
        imageFile = va_arg(ap, std::string);
        createTexture(imageFile);
    }
    va_end(ap);
}

TRSTexture::~TRSTexture()
{

}

int TRSTexture::createTexture(const std::string& imageFile, const std::string& sampleName/* = ""*/)
{
    size_t index = m_nTextures.size();
    m_nTextures.push_back(0);
    m_sImageFileNames.push_back(imageFile);
    std::string strSampleName = sampleName;
    if (strSampleName.empty())
    {
        strSampleName = std::string(s_TextUnitPrefix) + std::to_string(index);
    }
    m_sSampleNames.push_back(strSampleName);
    glGenTextures(1, &(m_nTextures[index]));
    glBindTexture(GL_TEXTURE_2D, m_nTextures[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    int width, height, channel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* source = stbi_load(imageFile.c_str(), &width, &height, &channel, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, source);
    glGenerateMipmap(GL_TEXTURE_2D);

    //最后释放图片内存
    stbi_image_free(source);
    return m_nTextures[index];
}

void TRSTexture::activeAllTextures()
{
    size_t nCount = m_nTextures.size();
    for (size_t i=0; i<nCount; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_nTextures[i]);
    }
}

void TRSTexture::addSharedTexture(const TextureData& textureData)
{
    m_nTextures.push_back(textureData.id);
    m_sImageFileNames.push_back(textureData.strImageFile);
    m_sSampleNames.push_back(textureData.strSampleName);
}

bool TRSTexture::getTextureDataByName(const std::string& imageFile, TextureData& outData)
{
    for (int i=0;i<m_nTextures.size(); i++)
    {
        if (m_sImageFileNames[i] == imageFile)
        {
            outData.id = m_nTextures[i];
            outData.strImageFile = m_sImageFileNames[i];
            outData.strSampleName = m_sSampleNames[i];
            return true;
        }
    }
    return false;
}

int TRSTexture::count()
{
    return m_nTextures.size();
}

std::vector<std::string> TRSTexture::getSampleNames()
{
    return m_sSampleNames;
}
