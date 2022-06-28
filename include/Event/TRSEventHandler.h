#pragma once
#include "Core\TRSExport.h"

class TRS_EXPORT TRSEventHandler
{
public:
    TRSEventHandler();

    virtual ~TRSEventHandler();

    virtual void processWindowSizeChange(int width, int height);

    virtual void processLeftMousePress(double xpos, double ypos, int mods);
    virtual void processLeftMouseRelease(double xpos, double ypos, int mods);

    virtual void processMiddleMousePress(double xpos, double ypos, int mods);
    virtual void processMiddleMouseRelease(double xpos, double ypos, int mods);

    virtual void processRightMousePress(double xpos, double ypos, int mods);
    virtual void processRightMouseRelease(double xpos, double ypos, int mods);

    // we use return value to shortcircuit the other three sub functions. default return value is false.
    virtual bool processMouseMove(double xpos, double ypos, int mods);
    virtual void processLeftMouseMove(double xpos, double ypos, int mods);
    virtual void processMiddleMouseMove(double xpos, double ypos, int mods);
    virtual void processRightMouseMove(double xpos, double ypos, int mods);

    virtual void processMouseScroll(double xScroll, double yScroll);

    virtual void processKeyPress(int key);

    virtual void processKeyRelease(int key);
};

