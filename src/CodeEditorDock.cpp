#include <engine/ui/docks/CodeEditorDock.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void CodeEditorDock::Render()
{
    ImGui::PushID(path.c_str());
    ImGui::Begin("FILE");
    
    ImGui::End();
    ImGui::ShowDemoWindow();
    ImGui::PopID();
}
