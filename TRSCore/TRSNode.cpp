﻿#include "TRSNode.h"
#include "TRSStateset.h"



TRSNode::TRSNode()
    : m_mat()
    , m_UpdateFunc(nullptr)
    , m_matColor(s_DefaultNodeColor)
{

}

TRSNode::~TRSNode()
{

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

std::shared_ptr<TRSVAO> TRSNode::getVAO() const
{
    return nullptr;
}

void TRSNode::draw()
{

}

void TRSNode::traverse(NodeVisitor& visitor)
{

}

std::shared_ptr<TRSStateSet> TRSNode::getStateSet() const
{
    return m_pStateSet;
}

std::shared_ptr<TRSStateSet> TRSNode::getOrCreateStateSet()
{
    if (!m_pStateSet)
    {
        m_pStateSet = TRSStateSetManager::instance()->createStateSet();
    }
    return m_pStateSet;
}

void TRSNode::setStateSet(std::shared_ptr<TRSStateSet> pStateSet)
{
    m_pStateSet = pStateSet;
}

void TRSNode::setUpdateCallBack(NodeUpdateFunc func)
{
    m_UpdateFunc = func;
}

NodeUpdateFunc TRSNode::getUpdateCallBack()
{
    return m_UpdateFunc;
}