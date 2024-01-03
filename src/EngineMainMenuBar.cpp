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

// Safe way to access current active code editor and perform an action on it.
#define EngineEditorDoAction(action, errMsg)                                    \
    auto& selected = EngineUI::Get()->GetDock<OpenedFilesDock>()->m_selected;   \
    auto& files    = EngineUI::Get()->GetDock<OpenedFilesDock>()->m_files;      \
    if (files.find(selected) != files.end()) { files[selected]->m_editor->action(); } else { LOG(errMsg); }

// Safe way to access current active code editor widget and perform action on it.
#define EngineEditorWidgetDoAction(action, errMsg)                              \
    auto& selected = EngineUI::Get()->GetDock<OpenedFilesDock>()->m_selected;   \
    auto& files    = EngineUI::Get()->GetDock<OpenedFilesDock>()->m_files;      \
    if (files.find(selected) != files.end()) { files[selected]->action(); } else { LOG(errMsg); }

// Abort action if path is invalid. (empty)
#define AbortIfPathIsInvalid(path) if (path.empty()) { LOG("Provided path is invalid!"); return; }

// Short version of creating menu item.
#define ImGuiBeginMenuItem(name, shortcut, callback, ...) if (ImGui::MenuItem(name, shortcut)) { callback(__VA_ARGS__); }

// Render text with provided font.
#define ImGuiTextWithFont(text, font) ImGui::PushFont(EngineAssetManager::Get()->m_fonts[font]); \
                                      ImGui::Text(text); \
                                      ImGui::PopFont();

EngineMainMenuBar::EngineMainMenuBar() 
    : m_openCreateNewProjectPopup(false)
    , m_createProjectModalFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar)
{
    LOG("EngineMainMenuBar::EngineMainMenuBar");
}   

void EngineMainMenuBar::Destroy()
{
    LOG("EngineMainMenuBar::Destroy");
}

void EngineMainMenuBar::OpenCreateNewProjectPopup()
{
    m_openCreateNewProjectPopup = true;
}

void EngineMainMenuBar::MenuActionExit()
{
    glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
}

void EngineMainMenuBar::MenuActionToggleDirectoryView()
{
    LOG("Perform menu action - View - Toggle Directory View");
    auto dirView = EngineUI::Get()->GetDock<DirectoryViewDock>();
    dirView->m_visible = !(dirView->m_visible);
}

void EngineMainMenuBar::MenuActionAbout()
{
    LOG("Perform menu action - Help - About");
}

void EngineMainMenuBar::Render()
{
    m_openCreateNewProjectPopup = false;

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::PushStyleColor(ImGuiCol_Border, { 0.3F, 0.3F, 0.3F, 1.0F });
        ImGui::PushStyleColor(ImGuiCol_Text, { 0.8F, 0.8F, 0.8F, 1.0F });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 15, 5 });
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);

        if (ImGui::BeginMenu("File"))
        {
            ImGuiBeginMenuItem(ICON_FA_GAMEPAD "  New Project", "Ctrl+P", OpenCreateNewProjectPopup);
            ImGui::Separator();
            ImGuiBeginMenuItem("New File", NULL, MenuActionNewFile);
            ImGuiBeginMenuItem("Open File", NULL, MenuActionOpenFile);
            ImGuiBeginMenuItem("Open Folder", NULL, MenuActionOpenFolder);
            ImGui::Separator();
            ImGuiBeginMenuItem("Save File", NULL, MenuActionSaveFile);
            ImGuiBeginMenuItem("Save File As", NULL, MenuActionSaveFileAs);
            ImGui::Separator();
            ImGuiBeginMenuItem("Exit...", "Ctrl+Q", MenuActionExit);
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit"))
        {
            ImGuiBeginMenuItem(ICON_FA_ROTATE_LEFT  "   Undo", "Ctrl+Z", MenuActionUndoAction);
            ImGuiBeginMenuItem(ICON_FA_ROTATE_RIGHT "   Redo", "Ctrl+Y", MenuActionRedoAction);
            ImGui::Separator();
            ImGuiBeginMenuItem(ICON_FA_SCISSORS "   Cut", "Ctrl+X", MenuActionCutAction);
            ImGuiBeginMenuItem(ICON_FA_COPY "   Copy", "Ctrl+C", MenuActionCopyAction);
            ImGuiBeginMenuItem(ICON_FA_PASTE "   Paste", "Ctrl+V", MenuActionPasteAction);
            ImGui::Separator();
            ImGuiBeginMenuItem(ICON_FA_MAGNIFYING_GLASS "   Find", "Ctrl+F", MenuActionFindAction);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGuiBeginMenuItem("Toggle Directory View", NULL, MenuActionToggleDirectoryView);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGuiBeginMenuItem("About", NULL, MenuActionAbout);
            ImGui::EndMenu();
        }

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);

        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 40);
        ImGuiTextWithFont(std::to_string((int)ImGui::GetIO().Framerate).c_str(), "JetBrains");
        ImGui::EndMainMenuBar();
    }

    if (m_openCreateNewProjectPopup)               
    {    
        ImGui::OpenPopup("Create New Project Modal");
    }
    CreateNewProjectPopup();
}


void EngineMainMenuBar::MenuActionUndoAction()
{
    LOG("EngineMainMenuBar -> MenuActionUndoAction");
    EngineEditorDoAction(Undo, "Unable to perform Undo action!");
}
void EngineMainMenuBar::MenuActionRedoAction()
{
    LOG("EngineMainMenuBar -> MenuActionRedoAction");
    EngineEditorDoAction(Redo, "Unable to perform Redo action!");
}

void EngineMainMenuBar::MenuActionCopyAction()
{
    LOG("EngineMainMenuBar -> MenuActionCopyAction");
    EngineEditorDoAction(Copy, "Unable to perform Copy action!");
}

void EngineMainMenuBar::MenuActionPasteAction()
{
    LOG("EngineMainMenuBar -> MenuActionPasteAction");
    EngineEditorDoAction(Paste, "Unable to perform Paste action!");
}

void EngineMainMenuBar::MenuActionCutAction()
{
    LOG("EngineMainMenuBar -> MenuActionCutAction");
    EngineEditorDoAction(Cut, "Unable to perform Cut action!");
}

void EngineMainMenuBar::MenuActionFindAction()
{
    LOG("EngineMainMenuBar -> MenuActionFindAction");
}

void EngineMainMenuBar::MenuActionNewFile()
{
    LOG("EngineMainMenuBar -> MenuActionNewFile");
    EngineUI::Get()->GetDock<OpenedFilesDock>()->OpenEmptyFile();
}

void EngineMainMenuBar::MenuActionOpenFile()
{
    LOG("EngineMainMenuBar -> MenuActionOpenFile");
    std::string path = EngineOpenFileDialog();
    AbortIfPathIsInvalid(path);
    EngineUI::Get()->GetDock<OpenedFilesDock>()->OpenFile(File(path));
}

void EngineMainMenuBar::MenuActionSaveFile()
{
    LOG("EngineMainMenuBar -> MenuActionSaveFile");
    EngineEditorWidgetDoAction(Save, "Failed to perform Save action.");
}

void EngineMainMenuBar::MenuActionSaveFileAs()
{
    LOG("EngineMainMenuBar -> MenuActionSaveFileAs");
    EngineEditorWidgetDoAction(SaveAs, "Failed to perform SaveAs action.");
}

void EngineMainMenuBar::MenuActionOpenFolder()
{
    LOG("EngineMainMenuBar::OpenFolder");
    std::string path = EngineOpenFolderDialog();
    AbortIfPathIsInvalid(path)
    EngineUI::Get()->GetDock<DirectoryViewDock>()->OpenFolder(path);
}

void EngineMainMenuBar::CreateNewProjectPopup()
{
    auto workSize   = ImGui::GetMainViewport()->WorkSize;
    auto modalSize  = ImVec2(500, 180);
    bool unsuedOpen = true;

    ImGui::SetNextWindowPos({ workSize.x / 2 - modalSize.x / 2, workSize.y / 2 - modalSize.y / 2 });
    ImGui::SetNextWindowSize(modalSize);

    ImGui::PushStyleColor(ImGuiCol_Border, { 0.3F, 0.3F, 0.3F, 1.0F });
    ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.05F, 0.05F, 0.05F, 1.0F });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10, 5 });

    if(ImGui::BeginPopupModal("Create New Project Modal", &unsuedOpen, m_createProjectModalFlags))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

        ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
        ImGui::Text("CRETE NEW PROJECT");
        ImGui::PopFont();
        
        ImGuiTextWithFont("Project Name", "JetBrains");

        ImGui::SetNextItemWidth(-1);
        static char sProjectName[30];
        ImGui::InputText("##InputProjName", sProjectName, 30);

        ImGuiTextWithFont("Location", "JetBrains");
        ImGui::SetNextItemWidth(-45);

        static char sProjectPath[255];

        ImGui::BeginDisabled();
        ImGui::InputText("##InputProjName1", sProjectPath, 255);
        ImGui::EndDisabled();

        ImGui::SameLine();        
        auto newpos = ImGui::GetCursorPos();
        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
        if(ImGui::Button("      "))
        {
            std::string path = EngineOpenFolderDialog();
            strcpy(sProjectPath, path.c_str());
        }     
        ImGui::PopStyleColor();
        ImGui::SetCursorPos({ newpos.x + 7, newpos.y + 5 });
        ImGui::Text(ICON_FA_FOLDER);

        ImGui::SetCursorPos({ImGui::GetWindowSize().x - 75, ImGui::GetWindowSize().y - 35});
        ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
        if(ImGui::Button("CREATE"))
        {
            OnCreateNewProjectConfirm(sProjectName, sProjectPath);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        ImGui::SetCursorPos({ImGui::GetWindowSize().x - 145, ImGui::GetWindowSize().y - 35});
        if(ImGui::Button("CANCEL"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();
}

void EngineMainMenuBar::OnCreateNewProjectConfirm(const std::string& name, const std::string& path)
{
    LOG("EngineMainMenuBar::OnCreateNewProjectConfirm:", name, path);

    std::filesystem::path p(path);
    if (!std::filesystem::exists(p))
    {
        LOG("PATH DOES NOT EXIST:", path);
        return;
    }

    std::filesystem::current_path(p);
    std::filesystem::create_directory(name.c_str());
    std::ofstream ofs(path + "/" + name + "/main.lua");
    ofs << "--- Remilia Scarlet Project : " << name << "\n"; 
    ofs << "print(\"Entry point.\")"; 
    ofs.close();

    ofs = std::ofstream(path + "/" + name + "/project.json");
    ofs.close();
    
    EngineUI::Get()->GetDock<DirectoryViewDock>()->OpenFolder(path + "/" + name);
}