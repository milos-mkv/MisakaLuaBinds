#include <engine/ui/EngineUIEventManager.hpp>

#include <utils/Logger.hpp>

PTR<EngineUIEventManager> EngineUIEventManager::Get()
{
    if (s_instance == nullptr)
    {
        s_instance = CreatePTR(EngineUIEventManager);
    }
    return s_instance;
}

EngineUIEventManager::EngineUIEventManager()
{
    LOG("EngineUIEventManager::EngineUIEventManager");
}

EngineUIEventManager::~EngineUIEventManager()
{
    LOG("EngineUIEventManager::~EngineUIEventManager");
}

