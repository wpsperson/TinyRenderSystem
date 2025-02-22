﻿#include "TRS/TRSTexture.h"

#include <iostream>
#include <cstdarg>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSDefConst.h"
#define STB_IMAGE_IMPLEMENTATION
#include "TRS/stb_image.h"


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
    GLenum format = GL_RGB;// default format is RGB
    if (channel == 1)
        format = GL_RED;
    else if (channel == 3)
        format = GL_RGB;
    else if (channel == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, source);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free the image memory
    stbi_image_free(source);
    return m_nTextures[index];
}

void TRSTexture::activeAllTextures(unsigned int program)
{
    size_t nCount = m_nTextures.size();
    for (size_t i=0; i<nCount; i++)
    {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + i));
        std::string sampleName = m_sSampleNames[i];
        int loc = glGetUniformLocation(program, sampleName.c_str());
        if (loc < 0)
        {
            std::cout << "WARNING::unifom sampler2D not found: " << sampleName << std::endl;
            continue;
        }
        glUniform1i(loc, static_cast<GLenum>(i));
        glBindTexture(GL_TEXTURE_2D, m_nTextures[i]);
    }
}

void TRSTexture::addSharedTexture(const TextureData& textureData)
{
    std::string textureFile = textureData.strImageFile;
    if ( std::find(m_sImageFileNames.begin(), m_sImageFileNames.end(), textureFile) == m_sImageFileNames.end() )
    {
        m_nTextures.push_back(textureData.id);
        m_sImageFileNames.push_back(textureData.strImageFile);
        m_sSampleNames.push_back(textureData.strSampleName);
    }
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
    return static_cast<int>(m_nTextures.size());
}

std::string TRSTexture::debugInfo()
{
    std::string strDebugInfo;
    for (int i = 0; i < m_nTextures.size(); i++)
    {
        std::string strCurTex ="texture id: " + std::to_string(m_nTextures[i]) + ", imageFile: " + m_sImageFileNames[i] + ", sampleName: " + m_sSampleNames[i];
        strDebugInfo += strCurTex + "\n";
    }
    return strDebugInfo;
}
