#include "TRSCharacterTexture.h"
#include <glad\glad.h>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

TRSCharacterTexture* TRSCharacterTexture::m_instance = nullptr;

TRSCharacterTexture::TRSCharacterTexture()
    : m_strategyMode(CommonChar_BigTex)
    , m_characters(nullptr)
    , m_textureID(0)
    , m_curXpos(0)
    , m_curYpos(0)
    , m_textureDimension(0)
    , m_singleFontSize(0)
{
    m_fontFile = std::getenv("SystemRoot") + std::string("/Fonts/simfang.ttf");
}

TRSCharacterTexture::~TRSCharacterTexture()
{
    if (m_characters)
    {
        delete[] m_characters;
    }
}

TRSCharacterTexture* TRSCharacterTexture::instance()
{
    if (!m_instance)
    {
        m_instance = new TRSCharacterTexture;
    }
    return m_instance;
}

void TRSCharacterTexture::freeInstance()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

void TRSCharacterTexture::setFontFile(const std::string& fontfile)
{
    m_fontFile = fontfile;
}

std::string TRSCharacterTexture::getFontFile() const
{
    return m_fontFile;
}

void TRSCharacterTexture::setFontStrategy(EnStrategy strategy)
{
    m_strategyMode = strategy;
}

bool TRSCharacterTexture::loadFreeType(std::string &error)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        error = "ERROR::FREETYPE: Could not init FreeType Library";
        return false;
    }

    if (FT_New_Face(ft, m_fontFile.c_str(), 0, &face))
    {
        error = "ERROR::FREETYPE: Failed to load font";
        return false;
    }

    if (FT_Select_Charmap(face, ft_encoding_unicode))
    {
        error = "ERROR::FT_Select_Charmap";
        return false;
    }
    //FT_Set_Pixel_Sizes(face, BitMapWeidth, BitMapHeight);
    if (m_singleFontSize == 0)
    {
        error = "single font size is zero, please generate texture first!";
        return false;
    }
    FT_Set_Char_Size(face, m_singleFontSize*64, m_singleFontSize*64, 72, 72);//cautions: 2nd and 3rd parameter is 1/64 pixel

    m_characters = new UniChar[65536];
    return true;
}

void TRSCharacterTexture::genTexture()
{
    GLint textureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureSize);
    int dimensionFontCount;
    if (m_strategyMode == CommonChar_BigTex)
    {
        dimensionFontCount = 64; //64*64=4096 common characters
    }
    else
    {
        dimensionFontCount = 256; //256*256=65536 unicode Chinese characters
    }
    int maxFontSize = textureSize / dimensionFontCount;
    if (maxFontSize < 16)
    {
        throw std::exception("available font size is too small to display Chinese characters!");
        return;
    }
    m_singleFontSize = std::min(maxFontSize, 64); // 64*64 is very accurate for single Chinese character font
    m_textureDimension = m_singleFontSize * dimensionFontCount;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    // generate texture image, but data is null.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_textureDimension, m_textureDimension, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int TRSCharacterTexture::getSingleFontSize()
{
    return m_singleFontSize;
}

int TRSCharacterTexture::getTexDimension()
{
    return m_textureDimension;
}

unsigned int TRSCharacterTexture::getTextureID()
{
    return m_textureID;
}

UniChar TRSCharacterTexture::getCharacter(wchar_t c)
{
    if (!m_characters)
    {
        throw std::exception("there is no character, please initialize first");
    }
    UniChar unichar = m_characters[c];
    if (unichar.ch == 0)
    {
        // get the glyph
        int index = FT_Get_Char_Index(face, c);
        FT_Load_Glyph(face, index, FT_LOAD_RENDER);
        unsigned char* data = face->glyph->bitmap.buffer;
        unichar.ch = c;
        unichar.x = m_curXpos;
        unichar.y = m_curYpos;
        unichar.w = face->glyph->bitmap.width;
        unichar.h = face->glyph->bitmap.rows;
        unichar.left = face->glyph->bitmap_left;
        unichar.top = face->glyph->bitmap_top;
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, unichar.x, unichar.y, unichar.w, unichar.h, GL_RED, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        m_characters[c] = unichar;
        // update current pos for next character
        m_curXpos += unichar.w + 1;
        if (m_curXpos + m_singleFontSize > m_textureDimension)
        {
            m_curXpos = 0;
            m_curYpos += m_singleFontSize;
            if (m_curYpos >= m_textureDimension)
            {
                throw std::exception("character font texture is out of range, may be too much character than reserved!");
            }
        }
    }
    return unichar;
}
