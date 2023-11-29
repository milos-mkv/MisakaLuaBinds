#ifndef ENGINE_UI_DOCK_COMPONENT
#define ENGINE_UI_DOCK_COMPONENT

#include <engine/components/RenderableComponent.hpp>

class EngineUIDock
{
public:
    virtual void Render()  = 0;
    virtual void Destroy() = 0;
};

#endif