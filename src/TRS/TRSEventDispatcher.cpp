﻿#include "TRS/TRSEventDispatcher.h"

#include "TRS/TRSEventHandler.h"
#include "TRS/TRSEventDefine.h"


TRSEventDispatcher::TRSEventDispatcher()
{

}

TRSEventDispatcher::~TRSEventDispatcher()
{

}

void TRSEventDispatcher::addEventHandler(TRSEventHandler* handler)
{
    m_pEventHandlers.push_back(handler);
}

void TRSEventDispatcher::removeEventHandler(TRSEventHandler* handler)
{
    m_pEventHandlers.remove(handler);
}

void TRSEventDispatcher::dispatchWindowSizeChange(int width, int height)
{
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processWindowSizeChange(width, height);
    }
}

void TRSEventDispatcher::dispatchLeftMousePress(int xpos, int ypos, int mods)
{
    m_leftMouseBtnPressed = true;
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processLeftMousePress(xpos, ypos, mods);
    }
}

void TRSEventDispatcher::dispatchLeftMouseRelease(int xpos, int ypos, int mods)
{
    m_leftMouseBtnPressed = false;
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processLeftMouseRelease(xpos, ypos, mods);
    }
}

void TRSEventDispatcher::dispatchMiddleMousePress(int xpos, int ypos, int mods)
{
    m_MiddleMouseBtnPressed = true;
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processMiddleMousePress(xpos, ypos, mods);
    }
}

void TRSEventDispatcher::dispatchMiddleMouseRelease(int xpos, int ypos, int mods)
{
    m_MiddleMouseBtnPressed = false;
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processMiddleMouseRelease(xpos, ypos, mods);
    }
}

void TRSEventDispatcher::dispatchRightMousePress(int xpos, int ypos, int mods)
{
    m_RightMouseBtnPressed = true;
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processRightMousePress(xpos, ypos, mods);
    }
}

void TRSEventDispatcher::dispatchRightMouseRelease(int xpos, int ypos, int mods)
{
    m_RightMouseBtnPressed = false;
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processRightMouseRelease(xpos, ypos, mods);
    }
}

void TRSEventDispatcher::dispatchMouseMove(int xpos, int ypos)
{
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        // here we use m_modifier to record the modifier
        bool processFinished = handler->processMouseMove(xpos, ypos, m_modifier);
        if (!processFinished)
        {
            if (m_leftMouseBtnPressed)
            {
                handler->processLeftMouseMove(xpos, ypos, m_modifier);
            }
            else if (m_MiddleMouseBtnPressed)
            {
                handler->processMiddleMouseMove(xpos, ypos, m_modifier);
            }
            else if (m_RightMouseBtnPressed)
            {
                handler->processRightMouseMove(xpos, ypos, m_modifier);
            }
        }
    }
}

void TRSEventDispatcher::dispatchMouseScroll(int scroll)
{
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processMouseScroll(scroll);
    }
}

void TRSEventDispatcher::dispatchKeyPress(int key, int mods)
{
    // mod modify
    if (key == TRS_KEY_LEFT_SHIFT || key == TRS_KEY_RIGHT_SHIFT)
    {
        m_modifier = m_modifier | TRS_MOD_SHIFT;
    }
    if (key == TRS_KEY_LEFT_CONTROL || key == TRS_KEY_RIGHT_CONTROL)
    {
        m_modifier = m_modifier | TRS_MOD_CONTROL;
    }
    if (key == TRS_KEY_LEFT_ALT || key == TRS_KEY_RIGHT_ALT)
    {
        m_modifier = m_modifier | TRS_MOD_ALT;
    }
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processKeyPress(key);
    }
}

void TRSEventDispatcher::dispatchKeyRelease(int key, int mods)
{
    if (key == TRS_KEY_LEFT_SHIFT || key == TRS_KEY_RIGHT_SHIFT)
    {
        m_modifier = m_modifier & ~TRS_MOD_SHIFT;
    }
    if (key == TRS_KEY_LEFT_CONTROL || key == TRS_KEY_RIGHT_CONTROL)
    {
        m_modifier = m_modifier & ~TRS_MOD_CONTROL;
    }
    if (key == TRS_KEY_LEFT_ALT || key == TRS_KEY_RIGHT_ALT)
    {
        m_modifier = m_modifier & ~TRS_MOD_ALT;
    }
    for (TRSEventHandler* handler : m_pEventHandlers)
    {
        handler->processKeyRelease(key);
    }
}
