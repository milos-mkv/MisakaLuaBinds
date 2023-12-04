#include <engine/ui/docks/OpenedFilesDock.hpp>
#include <engine/ui/EngineUI.hpp>

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <stack>

#include <imgui.h>
#include <imgui_internal.h>
#include <FontAwesomeIcons.hpp>

#include <utils/Logger.hpp>
#include <utils/Types.hpp>
#include <utils/Utils.hpp>

static int s_emptyTab = 1;

void OpenedFilesDock::Render()
{    
    // ImGui::ShowDemoWindow();
    ImGui::SetNextWindowClass(&m_windowClass);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });

    ImGui::PushStyleColor(ImGuiCol_Border, { 0.3, 0.3, 0.3, 1});
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0641,0.0641,0.0641, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0941,0.0941,0.0941, 1.00f});
    ImGui::Begin("Opened Files");
    
    if(m_files.size() > 0)
    {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0941,0.0941,0.0941, 1.00f));
        ImGui::BeginChild("ASDASDAS");
        ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 5);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 10 });

        auto cur = ImGui::GetCursorPos();

        ImGui::SetCursorPosY(cur.y + 39);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0641,0.0641,0.0641, 1.00f));

        ImGui::BeginChild("###Tabs childsas", { -1, -1}, true);
        // ImGui::Text("TESTST");
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::SetCursorPos({cur.x, cur.y});

        ImGui::Text("    ");
        ImGui::SameLine();

        if (ImGui::BeginTabBar("Open files tab bar", m_tabBarFlags)) 
        {
            for (const auto& [key, value] : m_files)
            {
                ImGui::PushID(key.c_str());
             
                if (ImGui::BeginTabItem((ICON_FA_CODE " " + value->m_fileName + "   ").c_str(), &value->m_alive, value->m_flags)) 
                { 
                    ImGui::SetCursorPos({
                        ImGui::GetCursorPosX() + 5,
                        ImGui::GetCursorPosY() + 5
                    });
                    m_selected = key;
                    // ImGui::Text("ASDASAA");
                    value->Render();
                    ImGui::EndTabItem();
                }

                if(value->m_flags == ImGuiTabItemFlags_SetSelected)
                {
                    value->m_flags = ImGuiTabItemFlags_None;
                }

                if (!value->m_alive)
                {
                    m_tabsToRemove.push(key);
                }
                ImGui::PopID();
            }
            ImGui::EndTabBar();

            while (m_tabsToRemove.size() > 0)
            {
                auto tab = m_tabsToRemove.top();
                LOG("Removing tab -", tab);
                m_files.erase(tab);
                m_tabsToRemove.pop();
            }  
        }
        ImGui::PopStyleVar(4);

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
    else 
    {
            ImGui::BeginChild("Tabs child", { -1, -1}, true);

        auto size = ImGui::CalcTextSize("Ctrl + N to create new file");
        ImGui::SetCursorPos({ ImGui::GetWindowSize().x / 2 - (size.x / 2), ImGui::GetWindowSize().y / 2 - (size.y / 2) });
        ImGui::TextDisabled("Ctrl + N to create new file");
            ImGui::EndChild();

    }
    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);
}

void OpenedFilesDock::Destroy()
{
    LOG("OpenedFilesDock::Destroy");
}

void OpenedFilesDock::OpenFile(const std::string& path, std::string fileName, std::string ext)
{
    LOG("OpenedFilesDock::OpenFile", path);
    std::unordered_map<std::string, PTR<CodeEditorWidget>>::iterator it = m_files.find(path.c_str());

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

        m_files[path] = CreatePTR(CodeEditorWidget, path, fileName, ext);
    }
    else
    {
        LOG(path, "already opened!");
    }
    m_selected = path;

    m_files[path]->m_flags = ImGuiTabItemFlags_SetSelected;
}

void OpenedFilesDock::OpenEmptyFile()
{
    LOG("OpenedFilesDock::OpenEmptyFile");

    std::string fileName = "Untitled " + std::to_string(s_emptyTab);
    m_files[fileName] = CreatePTR(CodeEditorWidget, "", fileName, "");
    s_emptyTab += 1;
}

OpenedFilesDock::OpenedFilesDock()
    : m_selected(""), m_files(std::unordered_map<std::string, PTR<CodeEditorWidget>>())
{
    LOG("OpenedFilesDock::OpenedFilesDock");
    
    

    m_windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplit 
                                           | ImGuiDockNodeFlags_NoDockingOverMe 
                                           | ImGuiDockNodeFlags_NoCloseButton 
                                           | ImGuiDockNodeFlags_NoTabBar;

    m_tabBarFlags = ImGuiTabBarFlags_AutoSelectNewTabs 
                  | ImGuiTabBarFlags_Reorderable 
                  | ImGuiTabBarFlags_DockNode
                  | ImGuiTabBarFlags_NoTabListScrollingButtons
                  ;
}

OpenedFilesDock::~OpenedFilesDock()
{
    LOG("OpenedFilesDock::~OpenedFilesDock");
}
