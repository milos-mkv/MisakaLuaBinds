#include <engine/ui/docks/OpenedFilesDock.hpp>

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <imgui_internal.h>
#include <FontAwesomeIcons.hpp>


#include <algorithm>
#include <FontAwesomeIcons.hpp>
#include <engine/gl/Texture.hpp>
#include <engine/ui/EngineUI.hpp>
#include <utils/Logger.hpp>
#include <thread>

void OpenedFilesDock::Render()
{
     ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingSplit
                                          | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar
                                          ;
    ImGui::SetNextWindowClass(&window_class);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 0));

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5f, 1.0f));
    ImGui::Begin("Opened Files");
    ImGui::PushFont(EngineUI::Get()->font);
    ImGui::Text("OPEN FILES");
    ImGui::PopFont();
    
    ImGui::Text("   ");
    ImGui::SameLine();

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
        for (const auto& [key, value] : EngineUI::Get()->m_codeEditors)
        {
            ImGui::PushID(value->path.c_str());

            if (ImGui::BeginTabItem((value->fileName + "   ").c_str(), &value->alive)) { 
                // switch to the newly selected tab
                value->Render();
                ImGui::EndTabItem();
            }
            ImGui::PopID();
        }
        ImGui::EndTabBar();
    }
    ImGui::PopStyleVar(4);


    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}
void OpenedFilesDock::Destroy()
{

}

OpenedFilesDock::OpenedFilesDock()
{

}

OpenedFilesDock::~OpenedFilesDock()
{

}