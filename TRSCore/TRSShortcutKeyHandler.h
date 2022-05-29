#pragma once
#include "TRSEventHandler.h"
#include "TRSExport.h"

class TRSContext;
class TRS_EXPORT TRSShortcutKeyHandler : public TRSEventHandler
{
public:
    TRSShortcutKeyHandler(TRSContext* context);

    virtual void processKeyPress(int key);

    virtual void processKeyRelease(int key);
private:
    TRSContext* m_context = nullptr;
};

