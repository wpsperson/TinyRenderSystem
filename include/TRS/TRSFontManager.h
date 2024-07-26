#pragma once

#include <string>
#include <unordered_map>

#include "TRS/TRSDefStruct.h"

class TRSFontManager
{
public:
    TRSFontManager();

    ~TRSFontManager();

    unsigned int fontTexutre() const noexcept;

    const AsciiCharInfo* asciiChar(char ch) const noexcept;

    bool loadAsciiCharInfo(std::string& error) noexcept;

    float textRelativeLength(const std::string& text);

private:
    unsigned int m_textureID = 0;
    std::unordered_map<char, AsciiCharInfo> m_fonts;
};


