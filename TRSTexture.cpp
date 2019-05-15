#include "TRSTexture.h"
#include <iostream>
#include <cstdarg>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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

void TRSTexture::createTexture(const std::string& imageFile)
{
    size_t index = m_nTextures.size();
    m_nTextures.push_back(0);
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
}

void TRSTexture::activeTexutres(std::vector<unsigned int> idxs)
{
    for (std::vector<unsigned int>::iterator itr = idxs.begin(); itr != idxs.end(); itr++)
    {
        if (*itr >= m_nTextures.size())
        {
            std::cout << "Texture Index out of range" << std::endl;
        }
        glActiveTexture(GL_TEXTURE0 + *itr);
        glBindTexture(GL_TEXTURE_2D, m_nTextures[*itr]);
    }
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

int TRSTexture::count()
{
    return m_nTextures.size();
}
