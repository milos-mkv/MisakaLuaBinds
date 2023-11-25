#include <engine/ui/EngineMainMenuBar.hpp>

#include <imgui.h>
#include <engine/ui/EngineAssetManager.hpp>
#include <nfd.h>
#include <utils/Logger.hpp>
#include <engine/ui/EngineUI.hpp>
#include <engine/ui/docks/DirectoryViewDock.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <FontAwesomeIcons.hpp>
#include <engine/ui/EngineUI.hpp>
#include <engine/gl/Texture.hpp>
#include <filesystem>
#include <string>
#include <utils/Logger.hpp>

EngineMainMenuBar::EngineMainMenuBar()
{

}

void EngineMainMenuBar::Destroy()
{

}

void EngineMainMenuBar::Render()
{
    bool openCreateNewProjectPopup = false;

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::PushStyleColor(ImGuiCol_Border, { 0.3, 0.3, 0.3, 1 });

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project", "Ctrl+P")) 
            { 
                openCreateNewProjectPopup = true;
            }
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) 
            {
                OpenFolder();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Ctrl+Q")) 
            {

            }
            ImGui::EndMenu();
        }
        ImGui::PopStyleColor();
        ImGui::EndMainMenuBar();
    }

    if (openCreateNewProjectPopup)               
    {    
        ImGui::OpenPopup("CREATE NEW PROJECT");
    }
    CreateNewProjectPopup();
}

void EngineMainMenuBar::OpenFolder()
{
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_PickFolder(NULL, &outPath);
        
    if (result == NFD_OKAY) 
    {
        LOG("Opening folder:", outPath);
        EngineUI::Get()->GetDock<DirectoryViewDock>()->OpenFolder(outPath);
    }
    else if (result == NFD_CANCEL) 
    {
        LOG("Open folder dialog canceled");
    }
    else 
    {
        LOG(NFD_GetError());
    }
}

static std::string GetOpenFolderPath()
{
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_PickFolder(NULL, &outPath);
        
    if (result == NFD_OKAY) 
    {
        LOG("Opening folder:", outPath);
        return std::string(outPath);
    }
    else if (result == NFD_CANCEL) 
    {
        LOG("Open folder dialog canceled");
        return "";
    }
    else 
    {
        LOG(NFD_GetError());
        return "";
    }
}


void EngineMainMenuBar::CreateNewProjectPopup()
{
    bool unsuedOpen = true;
    ImGui::SetNextWindowPos({
        ImGui::GetMainViewport()->WorkSize.x / 2 - 500 / 2,
        ImGui::GetMainViewport()->WorkSize.y / 2 - 300 / 2
    });
    ImGui::SetNextWindowSize({ 500, 300 });
    ImGui::PushFont(EngineAssetManager::Get()->fonts["JetBrains"]);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleColor(ImGuiCol_Border, { 0.51, 0.51, 0.51, 1});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

    auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

    if(ImGui::BeginPopupModal("CREATE NEW PROJECT", &unsuedOpen, flags))
    {
        auto cursave = ImGui::GetCursorPos();

        ImGui::SetCursorPos({-5, 0});
        ImGui::Image((ImTextureID) EngineAssetManager::Get()->textures["Project"].id, { 300, 300}, { 0, 0}, {1, 1}, {1, 1, 1, 0.5f});
        ImGui::SetCursorPos(cursave);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::SetCursorPosX(260);
        ImGui::Text("PROJECT NAME");

        ImGui::SetNextItemWidth(-15);
        ImGui::SetCursorPosX(260);

        static char sProjectName[30];
        ImGui::InputText("##InputProjName", sProjectName, 30);

        ImGui::SetCursorPosX(260);
        ImGui::Text("LOCATION");

        ImGui::SetCursorPosX(260);
        ImGui::SetNextItemWidth(-50);

        static char sProjectPath[255];
        ImGui::BeginDisabled();
        ImGui::InputText("##InputProjName1", sProjectPath, 255);
        ImGui::EndDisabled();
        ImGui::SameLine();        
        auto newpos = ImGui::GetCursorPos();
        if(ImGui::Button("  "))
        {
            std::string path = GetOpenFolderPath();
            strcpy(sProjectPath, path.c_str());
        }
        ImGui::SameLine();
        ImGui::SetCursorPos(newpos);
        ImGui::Image((ImTextureID) EngineAssetManager::Get()->textures["Folder Icon"].id, { 26, 26});

        ImGui::SetCursorPosX(260);
        ImGui::Text("PROJECT TYPE");

        const char* items[] = { "LUA", "C++" };
        static const char* current_item = items[0];
        
        ImGui::SetCursorPosX(260);
        ImGui::SetNextItemWidth(-15);
        if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(items[n], is_selected))
                    current_item = items[n];
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SetCursorPos({ImGui::GetWindowSize().x - 75, ImGui::GetWindowSize().y - 35});
        if(ImGui::Button("CREATE"))
        {
            OnCreateNewProjectConfirm(sProjectName, sProjectPath, current_item ? current_item : "");
        }
        ImGui::SameLine();
        ImGui::SetCursorPos({ImGui::GetWindowSize().x - 145, ImGui::GetWindowSize().y - 35});
        if(ImGui::Button("CANCEL"))
        {
            ImGui::CloseCurrentPopup();
        }
   
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::PopFont();
}

void EngineMainMenuBar::OnCreateNewProjectConfirm(const std::string& name, const std::string& path, const std::string& type)
{
    LOG("EngineMainMenuBar::OnCreateNewProjectConfirm:", name, path, type);

    std::filesystem::path p(path);
    if (!std::filesystem::exists(p))
    {
        LOG("PATH DOES NOT EXIST:", path);
        return;
    }

    std::filesystem::current_path(p);
    std::filesystem::create_directory(name.c_str());

    EngineUI::Get()->GetDock<DirectoryViewDock>()->OpenFolder((path + "/" + name).c_str());
}