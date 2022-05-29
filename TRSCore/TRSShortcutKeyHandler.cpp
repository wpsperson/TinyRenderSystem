#include "TRSShortcutKeyHandler.h"
#include "TRSEventDefine.h"
#include "TRSContext.h"
#include "TRSVisitors.h"


TRSShortcutKeyHandler::TRSShortcutKeyHandler(TRSContext* context, PolygonModeVisitor* polyModeVisitor)
    : m_context(context)
    , m_polyModeVisitor(polyModeVisitor)
{

}

void TRSShortcutKeyHandler::processKeyPress(int key)
{
    if (TRS_KEY_F1 == key)
    {
        m_polyModeVisitor->switchPolygonMode();
    }
}

void TRSShortcutKeyHandler::processKeyRelease(int key)
{
    if (TRS_KEY_ESCAPE == key)
    {
        m_context->setShouldClose(true);
    }
}
