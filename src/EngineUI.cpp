#include <engine/ui/EngineUI.hpp>
#include <engine/ui/docks/DirectoryViewDock.hpp>
#include <engine/ui/docks/CodeEditorDock.hpp>
#include <engine/ui/EngineMainMenuBar.hpp>
#include <utils/Logger.hpp>
#include <memory>

std::shared_ptr<EngineUI> EngineUI::Get()
{
    if (s_instance == nullptr)
        s_instance = std::make_shared<EngineUI>();
    return s_instance;
}

EngineUI::EngineUI()
{
    LOG("Initialize engine UI!");
    m_uiDocks[typeid(EngineMainMenuBar)] = std::make_shared<EngineMainMenuBar>();
    m_uiDocks[typeid(DirectoryViewDock)] = std::make_shared<DirectoryViewDock>();
}


EngineUI::~EngineUI()
{
    LOG("Destroy engine UI!");
}

void EngineUI::Render()
{
    for(const auto& [key, value] : m_uiDocks)
    {
        value->Render();
    }
}

void EngineUI::Destroy()
{
    LOG("EngineUI::Destroy");
    for(const auto& [key, value] : s_instance->m_uiDocks)
    {
        value->Destroy();
    }
}