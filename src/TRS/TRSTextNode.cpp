﻿#include "TRS/TRSTextNode.h"

#include "TRS/TRSDefGL.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSCharacterTexture.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSTexture.h"

TRSTextNode::TRSTextNode()
    : pos(G_ORIGIN)
    , right(G_XDIR)
    , up(G_YDIR)
{
    m_matColor = TRSVec4(0.5f, 1.0f, 0.5f, 1.0f);
    m_size = 0.1f;
}

TRSTextNode::~TRSTextNode()
{

}

NodeType TRSTextNode::nodeType() const
{
    return NodeType::ntTextNode;
}

void TRSTextNode::setText(const std::wstring& strText)
{
    if (m_text != strText)
    {
        m_text = strText;
        m_dirty = true;
    }
}

void TRSTextNode::setPos(const TRSVec3& textPos)
{
    if (pos != textPos)
    {
        pos = textPos;
        m_dirty = true;
    }
}

void TRSTextNode::setDir(const TRSVec3& textDirection)
{
    if (right != textDirection)
    {
        right = textDirection;
        m_dirty = true;
    }
}

void TRSTextNode::setUp(const TRSVec3& textUp)
{
    if (up != textUp)
    {
        up = textUp;
        m_dirty = true;
    }
}

void TRSTextNode::setSize(float size)
{
    m_size = size;
}

void TRSTextNode::initialize(TRSViewer*)
{
    if (m_dirty)
    {
        generateText();
        m_dirty = false;
    }
    m_shaded->uploadOnce();
}

void TRSTextNode::draw(RenderMode mode)
{
    TRSGeode::draw(mode);
}

std::string TRSTextNode::debugInfo()
{
    std::string strDebugInfo;
    if (m_pTexture)
    {
        strDebugInfo += m_pTexture->debugInfo();
    }
    return strDebugInfo;
}

void TRSTextNode::preProcess(RenderMode mode)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TRSTextNode::postProcess(RenderMode mode)
{
    glDisable(GL_BLEND);
}

void TRSTextNode::generateText()
{
    int textureDimension = TRSCharacterTexture::instance()->getTexDimension();
    int fontSize = TRSCharacterTexture::instance()->getSingleFontSize();
    float scale = m_size / fontSize;
    TRSVec3 curPos = pos;

    int size = static_cast<int>(m_text.size());
    std::vector<TRSPoint> vertices;
    std::vector<TRSVec2> UVs;
    std::vector<unsigned int> indices;
    unsigned int offset = 0;
    for (int i = 0; i < size; i++)
    {
        wchar_t c = m_text[i];
        UniChar unichar = TRSCharacterTexture::instance()->getCharacter(c);
        float rightOffset = unichar.left * scale;
        float upOffset = -(unichar.h - unichar.top) * scale;
        // four point in quad
        TRSVec3 leftBtm = curPos + right * rightOffset + up * upOffset;
        TRSVec3 leftTop = leftBtm + up * (unichar.h *scale);
        TRSVec3 rightBtm = leftBtm + right * (unichar.w * scale);
        TRSVec3 rightTop = leftBtm + right * (unichar.w * scale) + up*(unichar.h *scale);
        // texture coordinate
        float leftTC = float(unichar.x) / textureDimension;
        float topTC = float(unichar.y) / textureDimension;
        float rightTC = float(unichar.x + unichar.w) / textureDimension;
        float btmTC = float(unichar.y + unichar.h) / textureDimension;
        vertices.push_back(leftBtm);
        vertices.push_back(rightBtm);
        vertices.push_back(rightTop);
        vertices.push_back(leftTop);
        UVs.push_back(TRSVec2(leftTC, btmTC));
        UVs.push_back(TRSVec2(rightTC, btmTC));
        UVs.push_back(TRSVec2(rightTC, topTC));
        UVs.push_back(TRSVec2(leftTC, topTC));
        curPos += right * static_cast<float>(unichar.left + unichar.w) * scale;

        indices.emplace_back(offset + 0);
        indices.emplace_back(offset + 1);
        indices.emplace_back(offset + 2);
        indices.emplace_back(offset + 2);
        indices.emplace_back(offset + 3);
        indices.emplace_back(offset + 0);
        offset += 4;
    }
    // to do ,this snippet should before draw()
    m_ptCount = size * 6;
    m_shaded->setVertex(vertices);
    m_shaded->setUV(UVs);
    m_shaded->setIndices(indices);
}
