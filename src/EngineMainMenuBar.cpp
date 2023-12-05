#include <engine/ui/EngineMainMenuBar.hpp>

#include <engine/ui/docks/DirectoryViewDock.hpp>
#include <engine/ui/docks/OpenedFilesDock.hpp>
#include <engine/ui/EngineAssetManager.hpp>
#include <engine/ui/EngineUI.hpp>
#include <engine/ui/EngineUIActions.hpp>

#include <imgui.h>
#include <nfd/include/nfd.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <FontAwesomeIcons.hpp>

#include <filesystem>
#include <string>

#include <utils/Logger.hpp>

EngineMainMenuBar::EngineMainMenuBar()
{
    LOG("EngineMainMenuBar::EngineMainMenuBar");
}

void EngineMainMenuBar::Destroy()
{
    LOG("EngineMainMenuBar::Destroy");
}

void EngineMainMenuBar::Render()
{
    bool openCreateNewProjectPopup = false;

    if (ImGui::BeginMainMenuBar())
    {
        auto cur =ImGui::GetCursorPos();
        ImGui::PushStyleColor(ImGuiCol_Border, { 0.3, 0.3, 0.3, 1 });
        ImGui::PushStyleColor(ImGuiCol_Text, { 0.8, 0.8, 0.8, 1.0});
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 15);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 15);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 15, 5 });

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem(ICON_FA_GAMEPAD "  New Project", "Ctrl+P")) 
            { 
                openCreateNewProjectPopup = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("New File")) 
            { 
                NewFile();
            }
            if (ImGui::MenuItem("Open File")) 
            { 
                OpenFile();
            }
            if (ImGui::MenuItem("Open Folder")) 
            { 
                OpenFolder();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save File")) 
            { 
                SaveFile();
            }
            if (ImGui::MenuItem("Save File As")) 
            { 
                SaveFileAs();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Close File")) 
            { 
            }
            if (ImGui::MenuItem("Close All Files")) 
            { 
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit...", "Ctrl+Q")) 
            {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem(ICON_FA_ROTATE_LEFT "   Undo", "Ctrl+Z")) 
            { 
                LOG("Perform menu action - Edit - Undo");
            }
            if (ImGui::MenuItem(ICON_FA_ROTATE_RIGHT "   Redo", "Ctrl+Y")) 
            { 
                LOG("Perform menu action - Edit - Redo");
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_SCISSORS "   Cut", "Ctrl+X")) 
            {
                LOG("Perform menu action - Edit - Cut");
            }
            if (ImGui::MenuItem(ICON_FA_COPY "   Copy", "Ctrl+C")) 
            {
                LOG("Perform menu action - Edit - Copy");

            }
            if (ImGui::MenuItem(ICON_FA_PASTE "   Paste", "Ctrl+V")) 
            { 
                LOG("Perform menu action - Edit - Paste");
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_MAGNIFYING_GLASS "   Find", "Ctrl+F")) 
            { 
                LOG("Perform menu action - Edit - Find");
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Toggle Directory View")) 
            { 
                LOG("Perform menu action - View - Toggle Directory View");
                auto dirView = EngineUI::Get()->GetDock<DirectoryViewDock>();
                dirView->m_visible = !(dirView->m_visible);
            }
            if (ImGui::MenuItem("Toggle Toolbar")) 
            { 
                LOG("Perform menu action - View - Toggle Toolbar");
            }
            if (ImGui::MenuItem("Toggle Output Dock")) 
            { 
                LOG("Perform menu action - View - Toggle Output Dock");
            }
            ImGui::EndMenu();
        }

         if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About")) 
            { 
                LOG("Perform menu action - Help - About");
     
            }
          
            ImGui::EndMenu();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(4);

        cur.x = ImGui::GetWindowSize().x - 100;
        ImGui::SetCursorPos(cur);
        ImGui::Text(std::to_string((int)ImGui::GetIO().Framerate).c_str());
        ImGui::EndMainMenuBar();
    }

    if (openCreateNewProjectPopup)               
    {    
        ImGui::OpenPopup("CREATE NEW PROJECT");
    }
    CreateNewProjectPopup();
}

void EngineMainMenuBar::OpenFile()
{
    LOG("EngineMainMenuBar::OpenFile");
    std::string path = EngineOpenFileDialog();

    if (!path.empty())
    {
        EngineUI::Get()->GetDock<OpenedFilesDock>()->OpenFile(File(path));
    }
}

void EngineMainMenuBar::NewFile()
{
    LOG("EngineMainMenuBar::NewFile");
 
    EngineUI::Get()->GetDock<OpenedFilesDock>()->OpenEmptyFile();
    
}

void EngineMainMenuBar::SaveFile()
{
    LOG("EngineMainMenuBar::SaveFile");

    auto openedFilesDock = EngineUI::Get()->GetDock<OpenedFilesDock>();

    if (openedFilesDock->m_files.size() == 0)
        return;

    if (openedFilesDock->m_selected.empty())
        return;

    openedFilesDock->m_files[openedFilesDock->m_selected]->Save();
}

void EngineMainMenuBar::SaveFileAs()
{
    LOG("EngineMainMenuBar::SaveFile");

    auto openedFilesDock = EngineUI::Get()->GetDock<OpenedFilesDock>();

    if (openedFilesDock->m_files.size() == 0)
        return;

    if (openedFilesDock->m_selected.empty())
        return;

    openedFilesDock->m_files[openedFilesDock->m_selected]->SaveAs();
}

void EngineMainMenuBar::OpenFolder()
{
    LOG("EngineMainMenuBar::OpenFolder");
    std::string path = EngineOpenFolderDialog();

    if (!path.empty())
    {
        EngineUI::Get()->GetDock<DirectoryViewDock>()->OpenFolder(path.c_str());
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
    ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleColor(ImGuiCol_Border, { 0.51, 0.51, 0.51, 1});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

    auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

    if(ImGui::BeginPopupModal("CREATE NEW PROJECT", &unsuedOpen, flags))
    {
        auto cursave = ImGui::GetCursorPos();

        ImGui::SetCursorPos({-5, 0});
        ImGui::Image((ImTextureID) EngineAssetManager::Get()->m_textures["Project"].id, { 300, 300}, { 0, 0}, {1, 1}, {1, 1, 1, 0.5f});
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
        ImGui::Image((ImTextureID) EngineAssetManager::Get()->m_textures["Folder Icon"].id, { 26, 26});

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
            ImGui::CloseCurrentPopup();
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
    std::ofstream ofs(path + "/" + name + "/main.lua");
    ofs << "print(\"Hello world\")"; 
    ofs.close();

    ofs = std::ofstream(path + "/" + name + "/project.json");
    ofs.close();
    
    EngineUI::Get()->GetDock<DirectoryViewDock>()->OpenFolder(path + "/" + name);
}