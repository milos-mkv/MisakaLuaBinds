#ifndef MISAKA_ENGINE_UI_EVENT_MANAGER
#define MISAKA_ENGINE_UI_EVENT_MANAGER

#include <utils/Types.hpp>

class EngineUIEventManager
{
private:
    static inline PTR<EngineUIEventManager> s_instance = nullptr;

public:
    static PTR<EngineUIEventManager> Get();

    EngineUIEventManager();
   ~EngineUIEventManager();

};

#endif