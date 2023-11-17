#ifndef MISAKA_ENGINE_DIRECTORY_VIEW_DOCK
#define MISAKA_ENGINE_DIRECTORY_VIEW_DOCK

#include "../EngineUIDockComponent.hpp"
#include <utils/Logger.hpp>

class DirectoryViewDock : public EngineUIDockComponent
{
public:

    virtual void Render() override;
    DirectoryViewDock();
   ~DirectoryViewDock() { LOG("DESTROY"); } 
};

#endif