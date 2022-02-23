/*!  
 *@brief    初始配置和上下文
 *@author   wangps
 *@date     2019年5月3日
 */
 
#pragma once
#include "TRSExport.h"
#include "TRSContext.h"

struct GLFWwindow;

class TRS_EXPORT GLFWContext : public TRSContext
{
public:
    GLFWContext();
    ~GLFWContext();

    bool initContext() override;

    void connectEventDispatcher(TRSEventDispatcher* eventDispatcher)  override;

    bool shouldClose() override;

    void swapBuffer() override;

    static TRSEventDispatcher* getDispatcher();

private:
    GLFWwindow* m_window = nullptr;
    static TRSEventDispatcher* m_eventDispatcher;
};

struct GLFWwindow;
void TRSWindowChangeCallBack(GLFWwindow* window, int w, int h);
void TRSMouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods);
void TRSMouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);
void TRSKeyboardCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
void TRSMouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);
