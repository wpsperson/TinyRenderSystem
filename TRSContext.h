#pragma once

class TRSEventDispatcher;
class TRSContext
{
public:
    virtual bool initContext() = 0;

    virtual void connectEventDispatcher(TRSEventDispatcher* eventDispatcher) = 0;

    virtual bool shouldClose() = 0;

    virtual void swapBuffer() = 0;
};

