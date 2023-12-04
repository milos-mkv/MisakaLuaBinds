#include <engine/ui/EngineUI.hpp>
#include <engine/ui/EngineAssetManager.hpp>

#include <engine/ui/docks/DirectoryViewDock.hpp>
#include <engine/ui/docks/CodeEditorDock.hpp>
#include <engine/ui/docks/OpenedFilesDock.hpp>
#include <engine/ui/docks/StatusBarDock.hpp>
#include <engine/ui/docks/OutputDock.hpp>

#include <engine/ui/EngineMainMenuBar.hpp>

#include <utils/Logger.hpp>

PTR<EngineUI> EngineUI::Get()
{
    if (s_instance == nullptr) 
        s_instance = CreatePTR(EngineUI);
    return s_instance;
}

EngineUI::EngineUI()
{
    LOG("EngineUI::EngineUI");

    m_assetManager = EngineAssetManager::Get();

    m_uiDocks[typeid(EngineMainMenuBar)] = CreatePTR(EngineMainMenuBar);
    m_uiDocks[typeid(DirectoryViewDock)] = CreatePTR(DirectoryViewDock);
    m_uiDocks[typeid(OpenedFilesDock)]   = CreatePTR(OpenedFilesDock);
    m_uiDocks[typeid(StatusBarDock)]     = CreatePTR(StatusBarDock);
    m_uiDocks[typeid(OutputDock)]        = CreatePTR(OutputDock);
}


EngineUI::~EngineUI()
{
    LOG("EngineUI::~EngineUI");
}

void EngineUI::Render()
{
    for (const auto& [key, value] : m_uiDocks)
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
    m_assetManager->Destroy();
}