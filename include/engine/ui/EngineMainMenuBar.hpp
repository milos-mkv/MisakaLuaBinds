#ifndef MISAKA_ENGINE_MAIN_MENU_BAR
#define MISAKA_ENGINE_MAIN_MENU_BAR

#include <engine/ui/EngineUIDock.hpp>

class EngineMainMenuBar : public EngineUIDock
{
public:
    void Render() override;
    void Destroy() override {}

    EngineMainMenuBar() {}
    ~EngineMainMenuBar() {}

    void OpenFolder();
};

#endif