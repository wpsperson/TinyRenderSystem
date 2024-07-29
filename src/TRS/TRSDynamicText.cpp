#include "TRS/TRSDynamicText.h"

#include "TRS/TRSDefGL.h"
#include "TRS/TRSCamera.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSFontManager.h"
#include "TRS/TRSViewer.h"
#include "TRS/TRSDefStruct.h"


TRSDynamicText::TRSDynamicText()
{
}

TRSDynamicText::~TRSDynamicText()
{
}

NodeType TRSDynamicText::nodeType() const
{
    return NodeType::ntDynamicText;
}

void TRSDynamicText::setText(const std::string& strText)
{
    m_text = strText;
    m_needRebuild = true;
}

void TRSDynamicText::setOrigin(const TRSVec3& textPos)
{
    m_origin = textPos;
    m_needRebuild = true;
}

void TRSDynamicText::setFontSize(float size)
{
    m_fontSize = size;
    m_needRebuild = true;
}

void TRSDynamicText::initialize(TRSViewer* viewer)
{
    dynamicUpdate(viewer);

    // initialize bounding box
    TRSFontManager* fontMgr = viewer->getFontMgr();
    float relativeLen = fontMgr->textRelativeLength(m_text);
    float extend = relativeLen * m_fontSize / 2;
    m_box = TRSBox(m_origin, extend);
}

TRSBox TRSDynamicText::boundingBox() const
{
    TRSBox result;
    result.mergeBox(m_box);
    result.mergeBox(m_shaded->boundingBox());
    return result;
}

void TRSDynamicText::draw(RenderMode mode)
{
    TRSGeode::draw(mode);
}

void TRSDynamicText::dynamicUpdate(TRSViewer* viewer)
{
    TRSCamera* camera = viewer->getCamera();
    TRSFontManager* fontMgr = viewer->getFontMgr();
    bool changed = generateText(camera, fontMgr);
    if (changed)
    {
        m_shaded->setNeedUpload();
    }
    m_shaded->uploadOnce();
}

void TRSDynamicText::preProcess(RenderMode mode)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TRSDynamicText::postProcess(RenderMode mode)
{
    glDisable(GL_BLEND);
}

bool TRSDynamicText::generateText(TRSCamera* camera, TRSFontManager *fontMgr)
{
    const TRSVec3& right = camera->getRight();
    const TRSVec3& up = camera->getUp();
    if (!m_needRebuild && m_right == right && m_up == up)
    {
        return false;
    }
    m_right = right;
    m_up = up;
    m_shaded->reset();
    std::vector<TRSVec3> vertices;
    std::vector<TRSVec2> uvs;
    std::vector<unsigned int> indices;

    unsigned int index = 0;
    int text_count = static_cast<int>(m_text.size());
    float relativeLen = fontMgr->textRelativeLength(m_text);
    TRSPoint anchor = movePoint(m_origin, -(relativeLen * m_fontSize) / 2, -m_fontSize / 2);
    for (int idx = 0; idx < text_count; idx++)
    {
        char ch = m_text[idx];
        if (' ' == ch)
        {
            anchor = movePoint(anchor, m_fontSize, 0.0f);
        }
        const AsciiCharInfo* info = fontMgr->asciiChar(ch);
        TRSPoint pt0 = movePoint(anchor, info->offset_x * m_fontSize, info->offset_y * m_fontSize);
        TRSPoint pt1 = movePoint(pt0, info->width * m_fontSize, 0.0f);
        TRSPoint pt2 = movePoint(pt0, info->width * m_fontSize, info->height * m_fontSize);
        TRSPoint pt3 = movePoint(pt0, 0.0f, info->height * m_fontSize);
        anchor = movePoint(anchor, info->stride * m_fontSize, 0.0f);
        vertices.emplace_back(pt0);
        vertices.emplace_back(pt1);
        vertices.emplace_back(pt2);
        vertices.emplace_back(pt3);

        uvs.emplace_back(info->uv_min_x, info->uv_min_y);
        uvs.emplace_back(info->uv_max_x, info->uv_min_y);
        uvs.emplace_back(info->uv_max_x, info->uv_max_y);
        uvs.emplace_back(info->uv_min_x, info->uv_max_y);

        indices.emplace_back(index + 0);
        indices.emplace_back(index + 1);
        indices.emplace_back(index + 2);
        indices.emplace_back(index + 2);
        indices.emplace_back(index + 3);
        indices.emplace_back(index + 0);
        index += 4;
    }
    m_shaded->setVertex(vertices);
    m_shaded->setUV(uvs);
    m_shaded->setIndices(indices);
    m_needRebuild = false;
    return true;
}

TRSPoint TRSDynamicText::movePoint(const TRSPoint& origin, float distx, float disty)
{
    return origin + m_right * distx + m_up * disty;
}
