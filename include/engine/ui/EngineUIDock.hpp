#ifndef ENGINE_UI_DOCK_COMPONENT
#define ENGINE_UI_DOCK_COMPONENT

#include <engine/components/RenderableComponent.hpp>

class EngineUIDock : public RenderableComponent
{
public:
    virtual void Render() override { /* To override in child class. */ } 
    virtual void Destroy() = 0;
};

#endif