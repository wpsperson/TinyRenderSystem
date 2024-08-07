﻿#include "TRS/TRSNode.h"

#include "TRS/TRSDefConst.h"


TRSNode::TRSNode()
    : m_mat()
    , m_UpdateFunc(nullptr)
    , m_matColor(s_DefaultNodeColor)
{
}

TRSNode::~TRSNode()
{

}

NodeType TRSNode::nodeType() const
{
    return NodeType::ntUnknow;
}

void TRSNode::initialize(TRSViewer* viewer)
{
}

void TRSNode::traverse(NodeVisitor& visitor)
{

}

TRSBox TRSNode::boundingBox() const
{
    return TRSBox();
}

TRSMatrix TRSNode::getMatrix()
{
    return m_mat;
}

void TRSNode::setMatrix(const TRSMatrix& mat)
{
    m_mat = mat;
}

TRSVec4 TRSNode::getColor() const
{
    return m_matColor;
}

void TRSNode::setColor(const TRSVec4& color)
{
    m_matColor = color;
}

void TRSNode::setVisible(bool vis)
{
    m_visible = vis;
}

bool TRSNode::visible() const
{
    return m_visible;
}

void TRSNode::setName(const std::string& name)
{
    m_name = name;
}

const std::string& TRSNode::name() const
{
    return m_name;
}

void TRSNode::setUpdateCallBack(NodeUpdateFunc func)
{
    m_UpdateFunc = func;
}

NodeUpdateFunc TRSNode::getUpdateCallBack()
{
    return m_UpdateFunc;
}
