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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);

    ImGui::PushStyleColor(ImGuiCol_Border, COLOR_BORDER_4);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, COLOR_CHILD_BG_4);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, COLOR_CHILD_BG_4);

    ImGui::Begin("Opened Files");
    
    auto curposs = ImGui::GetCursorPos();

    if(m_files.size() > 0)
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, COLOR_CHILD_BG_4);
        ImGui::BeginChild("Tabs in display");
        
        ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 10 });


        if (ImGui::BeginTabBar("Open files tab bar", m_tabBarFlags)) 
        {
            for (const auto& [key, value] : m_files)
            {
                ImGui::PushID(key.c_str());

                auto name = (value->m_file.Name().empty()) ? key : value->m_file.Name();

                if (ImGui::BeginTabItem((ICON_FA_CODE " " + name + "   ").c_str(), &value->m_alive, value->m_flags)) 
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

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
    else 
    {
        ImGui::SetCursorPosY(curposs.y + 38);
        auto size = ImGui::CalcTextSize("Ctrl + N to create new file");
        ImGui::SetCursorPos({ ImGui::GetWindowSize().x / 2 - (size.x / 2), ImGui::GetWindowSize().y / 2 - (size.y / 2) });
        ImGui::TextDisabled("Ctrl + N to create new file");
    }

    RenderRunBar(curposs);

    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
}

void OpenedFilesDock::Destroy()
{
    LOG("OpenedFilesDock::Destroy");
}

void OpenedFilesDock::RenderRunBar(ImVec2 curpos)
{
    curpos.x = ImGui::GetWindowSize().x - 106;
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
}

void OpenedFilesDock::OpenFile(const File& file)
{
    LOG("OpenedFilesDock::OpenFile", file.Path());
    auto it = m_files.find(file.Path());

    if (it == m_files.end())
    {
        m_files[file.Path()] = CreatePTR(CodeEditorWidget, file);
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
    m_files["Untitled " + std::to_string(s_emptyTab++)] = CreatePTR(CodeEditorWidget, File());
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
