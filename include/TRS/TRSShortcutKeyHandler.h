#pragma once
#include "TRS/TRSEventHandler.h"
#include "TRS/TRSExport.h"

class TRSContext;
class PolygonModeVisitor;
class TRS_EXPORT TRSShortcutKeyHandler : public TRSEventHandler
{
public:
    TRSShortcutKeyHandler(TRSContext* context, PolygonModeVisitor* polyModeVisitor);

    virtual void processKeyPress(int key);

    virtual void processKeyRelease(int key);
private:
    TRSContext* m_context = nullptr;
    PolygonModeVisitor* m_polyModeVisitor = nullptr;
};

