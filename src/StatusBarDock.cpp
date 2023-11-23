#include <engine/ui/docks/StatusBarDock.hpp>


StatusBarDock::StatusBarDock()
{

}

void StatusBarDock::Render()
{
//     ImGui::SetNextWindowSize({ -1, 50 });
//     ImGui::Begin("Status Bar");
//     ImGui::Text("Status Bar");
//     ImGui::End();
    ImGui::ShowDemoWindow();
}

void StatusBarDock::Destroy()
{

}