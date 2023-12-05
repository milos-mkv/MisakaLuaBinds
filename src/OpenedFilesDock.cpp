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

void OpenedFilesDock::Render()
{    
    ImGui::SetNextWindowClass(&m_windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);

    ImGui::PushStyleColor(ImGuiCol_Border, COLOR_BORDER_4);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, COLOR_CHILD_BG_4);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, COLOR_BG_4);

    ImGui::Begin("Opened Files");
    
    if(m_files.size() > 0)
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, COLOR_BG_4);
        ImGui::BeginChild("Tabs in display");
        
        ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 5);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 10 });

        auto cur = ImGui::GetCursorPos();

        ImGui::SetCursorPosY(cur.y + 38);
        
        ImGui::PushStyleColor(ImGuiCol_ChildBg, COLOR_CHILD_BG_4);

        ImGui::BeginChild("ActiveTabFrameBorderWindow", { -1, -1 }, true);
        ImGui::EndChild();

        ImGui::PopStyleColor();

        ImGui::SetCursorPos({cur.x, cur.y});
        auto curpos = ImGui::GetCursorPos();

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

        
        curpos.x = ImGui::GetWindowSize().x - 104;
        ImGui::SetCursorPos(curpos);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, {0.0641,0.0641,0.0641, 1});
        ImGui::PushStyleColor(ImGuiCol_Button, {0.0641,0.6641,0.0641, 0});

        ImGui::BeginChild("Run Bar", { 104, 33}, true);
        
        ImGui::SetCursorPosY(curpos.y + 3);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3);

        auto c = ImGui::GetCursorPos();
        if (ImGui::Button("##ICON_FA_PLAY", { 30, 27}))
        {

        }
        c.x += 8;
        c.y += 5;
        ImGui::SetCursorPos(c);
        ImGui::Text(ICON_FA_PLAY);
        ImGui::SameLine();
        c.y -= 5;
        c.x += 25;
        ImGui::SetCursorPos(c);
        if (ImGui::Button("##ICON_FA_PAUSE", { 30, 27}))
        {}
        c.x += 9;
        c.y += 5;
        ImGui::SetCursorPos(c);

        ImGui::Text(ICON_FA_PAUSE);

        ImGui::SameLine();
        c.y -= 5;
        c.x += 25;
        ImGui::SetCursorPos(c);

        if (ImGui::Button("##ICON_FA_SLIDERS", { 30, 27}))
        {

        }
        c.x += 5;
        c.y += 5;
        ImGui::SetCursorPos(c);
        ImGui::Text(ICON_FA_SLIDERS);
        ImGui::EndChild();
        ImGui::PopStyleColor(2);

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
    else 
    {
        ImGui::BeginChild("No Tabs Display Child", { -1, -1 }, true);
        {
            auto size = ImGui::CalcTextSize("Ctrl + N to create new file");
            ImGui::SetCursorPos({ ImGui::GetWindowSize().x / 2 - (size.x / 2), ImGui::GetWindowSize().y / 2 - (size.y / 2) });
            ImGui::TextDisabled("Ctrl + N to create new file");
        }
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

void OpenedFilesDock::OpenFile(const File& file)
{
    LOG("OpenedFilesDock::OpenFile", file.Path());
    std::unordered_map<std::string, PTR<CodeEditorWidget>>::iterator it = m_files.find(file.Path().c_str());

    if (it == m_files.end())
    {
        m_files[file.Path()] = CreatePTR(CodeEditorWidget, file.Path(), file.Name(), file.Extn());
    }
    else
    {
        LOG(file.Path(), "already opened!");
    }

    m_selected = file.Path();
    m_files[file.Path()]->m_flags = ImGuiTabItemFlags_SetSelected;
}

void OpenedFilesDock::OpenEmptyFile()
{
    LOG("OpenedFilesDock::OpenEmptyFile");

    static int s_emptyTab = 1;
    std::string fileName = "Untitled " + std::to_string(s_emptyTab);
    m_files[fileName] = CreatePTR(CodeEditorWidget, "", fileName, "");
    s_emptyTab += 1;
}

OpenedFilesDock::OpenedFilesDock()
    : m_selected(""), 
      m_files(std::unordered_map<std::string, PTR<CodeEditorWidget>>())
{
    LOG("OpenedFilesDock::OpenedFilesDock");

    m_windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplit 
                                           | ImGuiDockNodeFlags_NoDockingOverMe 
                                           | ImGuiDockNodeFlags_NoCloseButton 
                                           | ImGuiDockNodeFlags_NoTabBar;

    m_tabBarFlags = ImGuiTabBarFlags_AutoSelectNewTabs 
                  | ImGuiTabBarFlags_Reorderable 
                  | ImGuiTabBarFlags_DockNode
                  | ImGuiTabBarFlags_NoTabListScrollingButtons;
}

OpenedFilesDock::~OpenedFilesDock()
{
    LOG("OpenedFilesDock::~OpenedFilesDock");
}
