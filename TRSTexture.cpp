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
    size_t index = textures.size();
    textures.push_back(0);
    glActiveTexture(GL_TEXTURE0 + index);//active index Texture Unit;
    glGenTextures(1, &(textures[index]));
    glBindTexture(GL_TEXTURE_2D, textures[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    int width, height, channel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* source = stbi_load(imageFile.c_str(), &width, &height, &channel, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, source);
    glGenerateMipmap(GL_TEXTURE_2D);

    //����ͷ�ͼƬ�ڴ�
    stbi_image_free(source);
}

void TRSTexture::activeTexutres(std::vector<unsigned int> idxs)
{
    for (std::vector<unsigned int>::iterator itr = idxs.begin(); itr != idxs.end(); itr++)
    {
        if (*itr >= textures.size())
        {
            std::cout << "Texture Index out of range" << std::endl;
        }
        glActiveTexture(GL_TEXTURE0 + *itr);
        glBindTexture(GL_TEXTURE_2D, textures[*itr]);
    }
}

void TRSTexture::activeAllTextures()
{
    size_t nCount = textures.size();
    for (size_t i=0; i<nCount; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}