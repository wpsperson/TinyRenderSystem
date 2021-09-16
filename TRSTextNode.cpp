#include "TRSTextNode.h"
#include "TRSVAO.h"
#include "TRSCharacterTexture.h"
#include "TRSStateSet.h"
#include "TRSShader.h"
#include "glad/glad.h"
#include "TRSTexture.h"

TRSTextNode::TRSTextNode()
    : pos(G_ORIGIN)
    , right(G_XDIR)
    , up(G_YDIR)
{
    m_matColor = TRSVec4(0.5f, 1.0f, 0.5f, 1.0f);
    m_size = 0.1;
    m_pVAO = std::make_shared<TRSVAO>();
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
    if (m_dirty)
    {
        preProcess();
        m_dirty = false;
    }
    glDrawArrays(GL_TRIANGLES, 0, m_ptCount);
}

std::shared_ptr<TRSVAO> TRSTextNode::getVAO() const
{
    return m_pVAO;
}

std::string TRSTextNode::debugInfo()
{
    TRSShader* pShader = m_pStateSet->getShader();
    TRSTexture* pTexture = m_pStateSet->getTexture();
    std::string strDebugInfo;
    strDebugInfo = "Geode VAO ID: " + std::to_string(m_pVAO->getVAO());
    strDebugInfo += pShader->debugInfo();
    strDebugInfo += pTexture->debugInfo();
    return strDebugInfo;
}

void TRSTextNode::preProcess()
{
    int textureDimension = TRSCharacterTexture::instance()->getTexDimension();
    int fontSize = TRSCharacterTexture::instance()->getSingleFontSize();
    float scale = m_size / fontSize;
    TRSVec3 curPos = pos;

    int size = m_text.size();
    float* vertexArray = new float[size * 6 * 5]; // each character need 6 point(2 triangle), each point need 5 float(3 coordinate and 2 texture coordinate)
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
        float curSixPtArray[6][5] = {
            { leftBtm[0], leftBtm[1], leftBtm[2],    leftTC, btmTC },
            { rightBtm[0], rightBtm[1], rightBtm[2], rightTC, btmTC },
            { leftTop[0], leftTop[1], leftTop[2],    leftTC, topTC },

            { leftTop[0], leftTop[1], leftTop[2],    leftTC, topTC },
            { rightBtm[0], rightBtm[1], rightBtm[2], rightTC, btmTC },
            { rightTop[0], rightTop[1], rightTop[2], rightTC, topTC },
        };
        std::memcpy(vertexArray + i * 30, curSixPtArray, sizeof(curSixPtArray));
        curPos += right * (unichar.left + unichar.w) * scale;
    }
    // to do ,this snippet should before draw()
    m_ptCount = size * 6 * 5;
    m_pVAO->createVAO(vertexArray, m_ptCount, EnVertexTexture);
}
