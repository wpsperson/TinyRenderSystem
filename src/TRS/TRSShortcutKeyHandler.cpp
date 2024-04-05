#include "TRS/TRSShortcutKeyHandler.h"
#include "TRS/TRSEventDefine.h"
// #include "Context\TRSContext.h"
#include "TRS/TRSVisitors.h"


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
        // m_context->setShouldClose(true);
    }
}
