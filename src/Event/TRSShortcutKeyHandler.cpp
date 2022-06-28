#include "Event\TRSShortcutKeyHandler.h"
#include "Event\TRSEventDefine.h"
#include "Context\TRSContext.h"
#include "Core\TRSVisitors.h"


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
