#ifndef MISAKA_ENGINE_STATUS_BAR_DOCK
#define MISAKA_ENGINE_STATUS_BAR_DOCK

#include <engine/ui/EngineUIDock.hpp>
#include <string>
#include <memory>
#include <TextEditor.h>

class StatusBarDock : public EngineUIDock
{
public:
    StatusBarDock();
    void Render() override;
    void Destroy() override;

};

#endif