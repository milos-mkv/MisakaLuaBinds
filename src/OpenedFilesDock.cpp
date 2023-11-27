#include <engine/ui/docks/OpenedFilesDock.hpp>

#include <string>

#include <vector>
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <imgui_internal.h>
#include <FontAwesomeIcons.hpp>

#include <vector>
#include <algorithm>
#include <FontAwesomeIcons.hpp>
#include <engine/gl/Texture.hpp>
#include <engine/ui/EngineUI.hpp>
#include <utils/Logger.hpp>
#include <thread>
#include <utils/Types.hpp>

static std::vector<std::string> SplitString(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } 
    while (pos < str.length() && prev < str.length());
    return tokens;
}

void OpenedFilesDock::Render()
{
     ImGuiWindowClass window_class; // | 
    window_class.DockNodeFlagsOverrideSet =  ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingOverMe | 
                                          ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar
                                          ;
    ImGui::SetNextWindowClass(&window_class);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 0));

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5f, 1.0f));
    ImGui::Begin("Opened Files");
    ImGui::PushFont(EngineAssetManager::Get()->fonts["JetBrains"]);
    ImGui::Text("OPEN FILES");
    ImGui::PopFont();

    // ImGui::SameLine();
    // ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 45);
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
    // ImGui::PushStyleColor(ImGuiCol_Text, { 0, 0.5, 0, 1});
    // ImGui::Button("  " ICON_FA_PLAY " ");
    // ImGui::PopStyleColor();
    
    // ImGui::Text("   ");
    // ImGui::SameLine();

    if(m_files.size() > 0)
    {

    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 10 });

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs 
    | ImGuiTabBarFlags_Reorderable 
    | ImGuiTabBarFlags_DockNode
    | ImGuiTabBarFlags_NoTabListScrollingButtons
    | ImGuiTabBarFlags_FittingPolicyScroll;

    if (ImGui::BeginTabBar("Open files tab bar", tab_bar_flags)) 
    {
        std::vector<std::string> toremove;
        for (const auto& [key, value] : m_files)
        {
            ImGui::PushID(value->path.c_str());

            if (ImGui::BeginTabItem((ICON_FA_CODE " " + value->fileName + "   ").c_str(), &value->alive)) { 
                // switch to the newly selected tab
                value->Render();
                ImGui::EndTabItem();
            }
            if (!value->alive)
            {
                LOG(key);
                toremove.push_back(key);
                LOG("REMOVE", value->fileName);
            }
            ImGui::PopID();
        }
        ImGui::EndTabBar();

        for(auto str : toremove)
            EngineUI::Get()->m_codeEditors.erase(str);

    }
    ImGui::PopStyleVar(4);

}

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}
void OpenedFilesDock::Destroy()
{

}


void OpenedFilesDock::OpenFile(const std::string& path, std::string fileName, std::string ext)
{
    LOG("OpenedFilesDock::OpenFile", path);
    std::unordered_map<std::string, PTR<CodeEditorDock>>::iterator it = m_files.find(path.c_str());

    if (it == m_files.end())
    {
        if (fileName.empty())
        {
            auto split = SplitString(path, "/");
            fileName = split[split.size() - 1];
        }
        if (ext.empty())
        {
            auto split = SplitString(path, ".");
            ext = "." + split[split.size() - 1];
        }

        m_files[path] = CreatePTR(CodeEditorDock, path, fileName, ext);
    }
    else
    {
        LOG(path, "already opened!");
    }
}

OpenedFilesDock::OpenedFilesDock()
{

}

OpenedFilesDock::~OpenedFilesDock()
{

}
