#pragma once
#include <list>

class TRSEventHandler;

class TRSEventDispatcher
{
public:
    TRSEventDispatcher();
    virtual ~TRSEventDispatcher();

    void addEventHandler(TRSEventHandler* handler);

    void removeEventHandler(TRSEventHandler* handler);

public:
    // dispatch all kinds of events:
    void dispatchWindowSizeChange(int width, int height);

    void dispatchLeftMousePress(double xpos, double ypos, int mods);
    void dispatchLeftMouseRelease(double xpos, double ypos, int mods);

    void dispatchMiddleMousePress(double xpos, double ypos, int mods);
    void dispatchMiddleMouseRelease(double xpos, double ypos, int mods);

    void dispatchRightMousePress(double xpos, double ypos, int mods);
    void dispatchRightMouseRelease(double xpos, double ypos, int mods);

    void dispatchMouseMove(double xpos, double ypos);

    void dispatchMouseScroll(double xScroll, double yScroll);

    void dispatchKeyPress(int key);

    void dispatchKeyRelease(int key);
protected:
    std::list<TRSEventHandler*> m_pEventHandlers;
    int m_modifier = 0;
    bool m_leftMouseBtnPressed = false;
    bool m_MiddleMouseBtnPressed = false;
    bool m_RightMouseBtnPressed = false;
};
