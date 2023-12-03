#include <engine/ui/docks/OutputDock.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <engine/ui/EngineUI.hpp>
#include <FontAwesomeIcons.hpp>

OutputDock::OutputDock()
{

}

void OutputDock::Render()
{
         ImGuiWindowClass window_class; // | 
    window_class.DockNodeFlagsOverrideSet =  ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingOverMe | 
                                          ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar
                                          ;
    ImGui::SetNextWindowClass(&window_class);


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });
    ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_Border, { 0.3, 0.3, 0.3, 1});
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0641,0.0641,0.0641, 1.00f));

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.5);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

    ImGui::Begin("OUTPUT");
    ImGui::BeginChild("ASDASD", {-1, -1}, true);
        ImGui::SetCursorPos({ 10, 5});

        ImGui::Text(ICON_FA_TERMINAL);
    ImGui::SameLine();
    ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
    ImGui::Text("OUTPUT");
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::End();

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(4);
}

void OutputDock::Destroy()
{

}