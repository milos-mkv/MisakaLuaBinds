#include <engine/ui/docks/OutputDock.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <engine/ui/EngineUI.hpp>

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
    ImGui::Begin("OUTPUT");
    ImGui::PushFont(EngineAssetManager::Get()->fonts["JetBrains"]);
    ImGui::Text("OUTPUT");
    // char buff[255];
    // ImGui::InputTextMultiline("##LOF", buff, 255, {-1, -1});
    ImGui::PopFont();
    ImGui::End();
}

void OutputDock::Destroy()
{

}