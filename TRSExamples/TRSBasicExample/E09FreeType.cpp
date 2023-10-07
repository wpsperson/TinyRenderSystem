#include "E09FreeType.h"
#include "Core/TRSConst.h"
#include "Core/TRSViewer.h"
#include "DataModel/TRSTextNode.h"
#include "DataModel/TRSGroup.h"

int CaseTextNode()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSTextNode> textNode = std::make_shared<TRSTextNode>();
    textNode->setText(L"中华人民共和国中央人民解放军");
    textNode->setDir(TRSVec3(0.866f, 0.5f, 0));
    textNode->setPos(TRSVec3(-1, 0, 0));
    std::shared_ptr<TRSTextNode> textNode2 = std::make_shared<TRSTextNode>();
    textNode2->setText(L"士不可以不弘毅，任重而道远");
    textNode2->setPos(TRSVec3(-1, 0, 0.5));
    textNode2->setColor(TRSVec4(0.8, 0.4, 0.4, 1));
    std::shared_ptr<TRSGroup> group = std::make_shared<TRSGroup>();
    group->addChild(textNode);
    group->addChild(textNode2);
    viewer->setSecenNode(group);
    viewer->run();
    return 0;
}