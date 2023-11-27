#include <engine/ui/EngineUI.hpp>
#include <engine/ui/EngineAssetManager.hpp>

#include <engine/ui/docks/DirectoryViewDock.hpp>
#include <engine/ui/docks/CodeEditorDock.hpp>
#include <engine/ui/docks/OpenedFilesDock.hpp>
#include <engine/ui/docks/StatusBarDock.hpp>
#include <engine/ui/docks/OutputDock.hpp>

#include <engine/ui/EngineMainMenuBar.hpp>
#include <utils/Logger.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <memory>

PTR<EngineUI> EngineUI::Get()
{
    if (s_instance == nullptr) 
        s_instance = CreatePTR(EngineUI);
    return s_instance;
}

EngineUI::EngineUI()
{
    LOG("EngineUI::EngineUI");

    m_assetManager = EngineAssetManager::Get(); // Load all assets.

    m_uiDocks[typeid(EngineMainMenuBar)] = CreatePTR(EngineMainMenuBar);
    m_uiDocks[typeid(DirectoryViewDock)] = CreatePTR(DirectoryViewDock);
    m_uiDocks[typeid(OpenedFilesDock  )] = CreatePTR(OpenedFilesDock  );
    m_uiDocks[typeid(StatusBarDock    )] = CreatePTR(StatusBarDock    );
}


EngineUI::~EngineUI()
{
    LOG("EngineUI::~EngineUI"); // NOTE: Use Destroy function to destruct object.
}

void EngineUI::Render()
{
    for (const auto& [key, value] : m_uiDocks)
    {
        value->Render();
    }
}

void EngineUI::OpenFile(const std::string& path, const std::string& fileName, const std::string& ext)
{
    LOG(path);
    std::unordered_map<std::string, std::shared_ptr<CodeEditorDock>>::iterator i = m_codeEditors.find(path);
    if (i == m_codeEditors.end())
    {
        m_codeEditors[path] = std::make_shared<CodeEditorDock>(path, fileName, ext);
    }
    else
    {
        m_codeEditors[path]->alive = true;
        // ImGui::SetWindowFocus(path.c_str());
        LOG(path, "already opened!");
    }
}

void EngineUI::Destroy()
{

    LOG("EngineUI::Destroy");
    for(const auto& [key, value] : s_instance->m_uiDocks)
    {
        value->Destroy();
    }
    EngineAssetManager::Get()->Destroy();

}