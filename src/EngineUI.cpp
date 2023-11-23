#include <engine/ui/EngineUI.hpp>

#include <engine/ui/docks/DirectoryViewDock.hpp>
#include <engine/ui/docks/CodeEditorDock.hpp>
#include <engine/ui/docks/OpenedFilesDock.hpp>
#include <engine/ui/docks/StatusBarDock.hpp>

#include <engine/ui/EngineMainMenuBar.hpp>
#include <utils/Logger.hpp>
#include <imgui.h>
#include <imgui_internal.h>
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
    m_uiDocks[typeid(OpenedFilesDock)]   = std::make_shared<OpenedFilesDock>();
    m_uiDocks[typeid(StatusBarDock)]   = std::make_shared<StatusBarDock>();

}


EngineUI::~EngineUI()
{
    LOG("Destroy engine UI!");
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
    std::unordered_map<std::string, std::shared_ptr<CodeEditorDock>>::iterator i = m_codeEditors.find(path);
    if (i == m_codeEditors.end())
    {
        m_codeEditors[path] = std::make_shared<CodeEditorDock>(path, fileName, ext);
    }
    else
    {
        m_codeEditors[path]->alive = true;
        ImGui::SetWindowFocus(path.c_str());
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
}