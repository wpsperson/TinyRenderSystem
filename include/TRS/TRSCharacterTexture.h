#pragma once
#include "TRS/TRSExport.h"
#include <string>

struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;

struct UniChar
{
    UniChar(): x(0), y(0), w(0), h(0), left(0), top(0), ch(0){ }
    int x; // remember (x,y) is on the left-top corner, not left-bottom corner.
    int y;
    int w;
    int h;
    int left;
    int top;
    wchar_t ch;
};


class TRSCharacterTexture
{
public:
    enum EnStrategy
    {
        CommonChar_BigTex = 0,  // In general, 4096 characters is enough. Texture will reserve only 4096 font, so each font size is more than 16*16
        AllChar_SmallTex,       // all Chinese will be used, each font size is 16*16
    };
public:
    static TRSCharacterTexture* instance();
    static void freeInstance();
    void setFontFile(const std::string& fontfile);
    std::string getFontFile() const;
    void setFontStrategy(EnStrategy strategy);
    bool loadFreeType(std::string &error);
    void genTexture();
    int getSingleFontSize();
    int getTexDimension();
    unsigned int getTextureID();
    UniChar getCharacter(wchar_t c);
private:
    TRSCharacterTexture();
    ~TRSCharacterTexture();
private:
    static TRSCharacterTexture* m_instance; // singleton
    UniChar* m_characters;      // UniChar array
    FT_Face face;               // freetype Face object
    std::string m_fontFile;     // designate which font we use, default is "C:/Windows/Fonts/simfang.ttf"
    unsigned int m_textureID;   // the texture id
    int m_strategyMode;         // designate which strategy we use to generate texture.
    int m_textureDimension;     // total texture dimension. may be 64 * singleFontSize
    int m_singleFontSize;       // each character dimension. prefer to 64
    int m_curXpos;              // current character left-top corner point x coordinate
    int m_curYpos;              // current character left-top corner point y coordinate
};

