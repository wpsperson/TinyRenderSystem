#include "TRSShortcutKeyHandler.h"
#include "TRSEventDefine.h"
#include "TRSContext.h"


TRSShortcutKeyHandler::TRSShortcutKeyHandler(TRSContext* context)
    : m_context(context)
{

}

void TRSShortcutKeyHandler::processKeyPress(int key)
{

}

void TRSShortcutKeyHandler::processKeyRelease(int key)
{
    if (TRS_KEY_ESCAPE == key)
    {
        m_context->setShouldClose(true);
    }
}
