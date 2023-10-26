#pragma once
#include <list>
#include "Core\TRSExport.h"

class TRSEventHandler;

class TRS_EXPORT TRSEventDispatcher
{
public:
    TRSEventDispatcher();
    virtual ~TRSEventDispatcher();

    void addEventHandler(TRSEventHandler* handler);

    void removeEventHandler(TRSEventHandler* handler);

public:
    // dispatch all kinds of events:
    void dispatchWindowSizeChange(int width, int height);

    void dispatchLeftMousePress(int xpos, int ypos, int mods);
    void dispatchLeftMouseRelease(int xpos, int ypos, int mods);

    void dispatchMiddleMousePress(int xpos, int ypos, int mods);
    void dispatchMiddleMouseRelease(int xpos, int ypos, int mods);

    void dispatchRightMousePress(int xpos, int ypos, int mods);
    void dispatchRightMouseRelease(int xpos, int ypos, int mods);

    void dispatchMouseMove(int xpos, int ypos);

    void dispatchMouseScroll(int scroll);

    void dispatchKeyPress(int key, int mods);

    void dispatchKeyRelease(int key, int mods);
protected:
    std::list<TRSEventHandler*> m_pEventHandlers;
    int m_modifier = 0;
    bool m_leftMouseBtnPressed = false;
    bool m_MiddleMouseBtnPressed = false;
    bool m_RightMouseBtnPressed = false;
};
