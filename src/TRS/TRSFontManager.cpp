#include "TRS/TRSFontManager.h"

#include <freetype/freetype.h>
#include "TRS/TRSDefGL.h"

#pragma warning(disable: 4996)


// most glyph width and height is from 0.7~0.8, so scale it to make font normalized.
static constexpr float kGlyphScale = 1.3f;

#if _WIN32
static const std::string kLocalFontFile = std::getenv("SystemRoot") + std::string("/Fonts/") + "Arial.ttf";
#else 
static const std::string kLocalFontFile = std::string("/usr/share/fonts/liberation/LiberationSans-Regular.ttf");
#endif

TRSFontManager::TRSFontManager()
{
}

TRSFontManager::~TRSFontManager()
{
}

unsigned int TRSFontManager::fontTexutre() const noexcept
{
    return m_textureID;
}

const AsciiCharInfo* TRSFontManager::asciiChar(char ch) const noexcept
{
    if (ch > 0 && ch < 128)
    {
        return &(m_fonts.at(ch));
    }
    return nullptr;
}

bool TRSFontManager::loadAsciiCharInfo(std::string& error) noexcept
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        error = "Failed to init FreeType library!";
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, kLocalFontFile.c_str(), 0, &face))
    {
        error = "Failed to load font: " + kLocalFontFile + "!";
        return false;
    }

    if (FT_Select_Charmap(face, ft_encoding_unicode))
    {
        error = "Failed to select charmap";
        return false;
    }
    int oneDimensionCharNum = 16; // ascii char num less than 256;
    int fontSize = 256;  // 64 pixel is accuracy
    int textureSize = fontSize * oneDimensionCharNum;
    GLint maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    if (maxTextureSize < textureSize)
    {
        textureSize = maxTextureSize;
        fontSize = textureSize / oneDimensionCharNum;
    }
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    // generate texture image, but data is null.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureSize, textureSize, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int current_x = 0;
    int current_y = 0;
    for (GLubyte ch = 0; ch < 128; ch++)
    {
        if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
        {
            error = "Failed to load Char";
            return false;
        }
        unsigned int width = face->glyph->bitmap.width;
        unsigned int height = face->glyph->bitmap.rows;
        glTexSubImage2D(GL_TEXTURE_2D, 0, current_x, current_y, width, height, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        float offset_to_bttm = static_cast<float>(face->glyph->bitmap.rows) - static_cast<float>(face->glyph->bitmap_top);
        AsciiCharInfo newCharacter;
        newCharacter.uv_min_x = static_cast<float>(current_x) / textureSize;
        newCharacter.uv_max_x = static_cast<float>(current_x + width) / textureSize;
        newCharacter.uv_min_y = static_cast<float>(current_y + height) / textureSize;
        newCharacter.uv_max_y = static_cast<float>(current_y) / textureSize;
        newCharacter.width = static_cast<float>(width) * kGlyphScale / fontSize;
        newCharacter.height = static_cast<float>(height) * kGlyphScale / fontSize;
        newCharacter.offset_x = static_cast<float>(face->glyph->bitmap_left) * kGlyphScale / fontSize;
        newCharacter.offset_y =  (-offset_to_bttm) * kGlyphScale / fontSize;
        newCharacter.stride = static_cast<float>(face->glyph->advance.x >> 6) * kGlyphScale / fontSize;
        m_fonts[ch] = newCharacter;

        current_x += fontSize;
        if (current_x + fontSize > textureSize)
        {
            current_x = 0;
            current_y += fontSize;
        }
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return true;
}
