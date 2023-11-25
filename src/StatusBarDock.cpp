#include <engine/ui/docks/StatusBarDock.hpp>


StatusBarDock::StatusBarDock()
{

}

void StatusBarDock::Render()
{
//     ImGui::SetNextWindowSize({ -1, 50 });
ImGui::SetNextWindowPos({ImGui::GetMainViewport()->Pos.x, ImGui::GetMainViewport()->Pos.y + 25});
ImGui::SetNextWindowSize({    ImGui::GetMainViewport()->Size.x
 , 25});
 bool a = true;
    ImGui::Begin("Status Bar", &a, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoScrollbar);
    // ImGui::Text("----------------------------------------------------------------------------------------------------------------------------------------------------");
    ImGui::End();
    // ImGui::ShowDemoWindow();
}

void StatusBarDock::Destroy()
{

}