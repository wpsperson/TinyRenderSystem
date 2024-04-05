#pragma once
#include "TRS/TRSExport.h"

class TRS_EXPORT TRSEventHandler
{
public:
    TRSEventHandler();

    virtual ~TRSEventHandler();

    virtual void processWindowSizeChange(int width, int height);

    virtual void processLeftMousePress(int xpos, int ypos, int mods);
    virtual void processLeftMouseRelease(int xpos, int ypos, int mods);

    virtual void processMiddleMousePress(int xpos, int ypos, int mods);
    virtual void processMiddleMouseRelease(int xpos, int ypos, int mods);

    virtual void processRightMousePress(int xpos, int ypos, int mods);
    virtual void processRightMouseRelease(int xpos, int ypos, int mods);

    // we use return value to shortcircuit the other three sub functions. default return value is false.
    virtual bool processMouseMove(int xpos, int ypos, int mods);
    virtual void processLeftMouseMove(int xpos, int ypos, int mods);
    virtual void processMiddleMouseMove(int xpos, int ypos, int mods);
    virtual void processRightMouseMove(int xpos, int ypos, int mods);

    virtual void processMouseScroll(int scroll);

    virtual void processKeyPress(int key);

    virtual void processKeyRelease(int key);
};

