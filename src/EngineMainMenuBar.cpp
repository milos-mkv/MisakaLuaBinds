#include <engine/ui/EngineMainMenuBar.hpp>

#include <imgui.h>
#include <nfd.h>
#include <utils/Logger.hpp>
#include <engine/ui/EngineUI.hpp>
#include <engine/ui/docks/DirectoryViewDock.hpp>

void EngineMainMenuBar::Render()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) { 
                
            }
            if (ImGui::MenuItem("Open Folder", "Ctrl+O")) 
            {
                OpenFolder();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Ctrl+Q")) 
            {
            //    glfwSetWindowShouldClose(Engine::window, true);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EngineMainMenuBar::OpenFolder()
{
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_PickFolder(NULL, &outPath);
        
    if (result == NFD_OKAY) {
        LOG("Opening folder:", outPath);
        EngineUI::Get()->GetDock<DirectoryViewDock>()->OpenFolder(outPath);
    }
    else if (result == NFD_CANCEL) {
        LOG("Open folder dialog canceled");
    }
    else {
        LOG(NFD_GetError());
    }
}