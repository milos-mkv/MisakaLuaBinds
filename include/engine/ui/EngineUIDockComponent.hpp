#ifndef ENGINE_UI_DOCK_COMPONENT
#define ENGINE_UI_DOCK_COMPONENT

#include <engine/components/RenderableComponent.hpp>

class EngineUIDockComponent : public RenderableComponent
{
public:
    virtual void Render() override { /* To override in child class. */ } 
};

#endif