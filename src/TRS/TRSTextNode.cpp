#include "TRS/TRSTextNode.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSCharacterTexture.h"
#include "TRS/TRSStateSet.h"
#include "TRS/TRSShader.h"
#include "TRS/glad/glad.h"
#include "TRS/TRSTexture.h"

TRSTextNode::TRSTextNode()
    : pos(G_ORIGIN)
    , right(G_XDIR)
    , up(G_YDIR)
{
    m_matColor = TRSVec4(0.5f, 1.0f, 0.5f, 1.0f);
    m_size = 0.1f;
    getOrCreateStateSet();
    m_pStateSet->getShader()->createProgram("shaders/FontsVertex.glsl", "shaders/FontsFragment .glsl");

    // deal texture
    unsigned int textureID = TRSCharacterTexture::instance()->getTextureID();
    TextureData texData(textureID, "all_unicode_texture", "ourTexture");
    m_pStateSet->getTexture()->addSharedTexture(texData);
}

TRSTextNode::~TRSTextNode()
{

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

void TRSTextNode::draw()
{
    glDrawArrays(GL_TRIANGLES, 0, m_ptCount);
}

void TRSTextNode::setActive()
{
    if (m_dirty)
    {
        generateText();
        m_dirty = false;
    }
    m_pMesh->meshBind();
}

std::string TRSTextNode::debugInfo()
{
    TRSShader* pShader = m_pStateSet->getShader();
    TRSTexture* pTexture = m_pStateSet->getTexture();
    std::string strDebugInfo;
    strDebugInfo += pShader->debugInfo();
    strDebugInfo += pTexture->debugInfo();
    return strDebugInfo;
}

void TRSTextNode::preProcess()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TRSTextNode::postProcess()
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
    //float* vertexArray = new float[size * 6 * 5]; // each character need 6 point(2 triangle), each point need 5 float(3 coordinate and 2 texture coordinate)
    for (int i = 0; i < size; i++)
    {
        wchar_t c = m_text[i];
        UniChar unichar = TRSCharacterTexture::instance()->getCharacter(c);
        float rightOffset = unichar.left * scale;
        float upOffset = -(unichar.h - unichar.top) * scale;
        // four point in quad
        TRSVec3 leftBtm = curPos + right * rightOffset + up * upOffset;
        TRSVec3 leftTop = leftBtm + up*(unichar.h *scale);
        TRSVec3 rightBtm = leftBtm + right * (unichar.w * scale);
        TRSVec3 rightTop = leftBtm + right * (unichar.w * scale) + up*(unichar.h *scale);
        // texture coordinate
        float leftTC = float(unichar.x) / textureDimension;
        float topTC = float(unichar.y) / textureDimension;
        float rightTC = float(unichar.x + unichar.w) / textureDimension;
        float btmTC = float(unichar.y + unichar.h) / textureDimension;
        vertices.push_back(TRSPoint(leftBtm[0], leftBtm[1], leftBtm[2])); UVs.push_back(TRSVec2(leftTC, btmTC));
        vertices.push_back(TRSPoint(rightBtm[0], rightBtm[1], rightBtm[2])); UVs.push_back(TRSVec2(rightTC, btmTC));
        vertices.push_back(TRSPoint(leftTop[0], leftTop[1], leftTop[2])); UVs.push_back(TRSVec2(leftTC, topTC));

        vertices.push_back(TRSPoint(leftTop[0], leftTop[1], leftTop[2])); UVs.push_back(TRSVec2(leftTC, topTC));
        vertices.push_back(TRSPoint(rightBtm[0], rightBtm[1], rightBtm[2])); UVs.push_back(TRSVec2(rightTC, btmTC));
        vertices.push_back(TRSPoint(rightTop[0], rightTop[1], rightTop[2])); UVs.push_back(TRSVec2(rightTC, topTC));
        curPos += right * static_cast<float>(unichar.left + unichar.w) * scale;
    }
    // to do ,this snippet should before draw()
    m_ptCount = size * 6;
    m_pMesh->setVertex(vertices);
    m_pMesh->setUV(UVs);
}
