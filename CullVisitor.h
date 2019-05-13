/*!
 *@brief     
 *@author     wangps
 *@date       2019Äê5ÔÂ11ÈÕ
 */

#pragma once
#include "TRSExport.h"
#include "NodeVisitor.h"
#include <map>
#include <vector>


class TRS_EXPORT CullVisitor : public NodeVisitor
{
public:
    CullVisitor();
    ~CullVisitor();
    virtual void execute(TRSNode* pNode);
    
    std::map<int, std::vector<TRSNode*>> getMapState2Node();

protected:
    std::map<int, std::vector<TRSNode*>> m_mapState2Node;
    
};
