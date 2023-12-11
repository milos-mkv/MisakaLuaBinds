#include <engine/ui/docks/DirectoryViewDock.hpp>

#include <engine/ui/EngineAssetManager.hpp>
#include <engine/ui/EngineUI.hpp>
#include <engine/ui/docks/OpenedFilesDock.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <FontAwesomeIcons.hpp>

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <thread>

#include <utils/Logger.hpp>

ImRect DirectoryViewDock::RecursivelyDisplayDirectoryNode(const PTR<DirectoryNode>& parentNode)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

	ImGui::PushID(parentNode->Path().c_str());
	if (parentNode->m_isDirectory)
	{
		if (ImGui::TreeNodeEx((" " ICON_FA_FOLDER " " + parentNode->Name()).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
		{
            // LOG(parentNode->Path());
            parentNode->Open();
            const float SmallOffsetX = -8.0f;
            ImDrawList* drawList     = ImGui::GetWindowDrawList();
            ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
            verticalLineStart.x     += SmallOffsetX;
            ImVec2 verticalLineEnd   = verticalLineStart;

            OpenContextMenu(parentNode, true);

			for (const auto& childNode : parentNode->m_children)
			{	
                const ImRect childRect = RecursivelyDisplayDirectoryNode(childNode);
                
                const float HorizontalTreeLineSize = 8.0f; 
                const float midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f + 25;
                drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), ImGui::GetColorU32({ 0.2, 0.2, 0.2, 1.0f }));
                verticalLineEnd.y = midpoint;
            }
            drawList->AddLine(verticalLineStart, verticalLineEnd, ImGui::GetColorU32({ 0.2, 0.2, 0.2, 1.0f }));

			ImGui::TreePop();
		}
        else 
        { 
            OpenContextMenu(parentNode, true); 
        }
	}
	else
	{
        auto startCursor = ImGui::GetCursorPos();

        ImGui::SetCursorPosX(0);

        ImGui::PushStyleColor(ImGuiCol_Button, (m_selectedFile == parentNode->Path()) ? ImVec4(0.25f, 0.25f, 0.25f, 1.00f) : ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

        if (ImGui::Button(("###  " + parentNode->Name()).c_str(), ImVec2(-1, 23)))
        {
            LOG("File selected:", parentNode->Path());
            m_selectedFile = parentNode->Path();

            EngineUI::Get()->GetDock<OpenedFilesDock>()->OpenFile(parentNode->m_file);
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        OpenContextMenu(parentNode, false);

        ImGui::SameLine();
        ImGui::SetCursorPos(startCursor);
        ImGui::Text(("        " ICON_FA_FILE_CODE "  "  + parentNode->Name()).c_str());
	}
	ImGui::PopID();
    return nodeRect;
}

DirectoryViewDock::DirectoryViewDock() : m_visible(true), m_removeFile(nullptr)
{
    LOG("DirectoryViewDock::DirectoryViewDock");

    m_windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;
    m_modalFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

    OpenFolder("..");
}

void DirectoryViewDock::Destroy()
{
    LOG("DirectoryViewDock::Destroy");
    // if (m_rootNode->m_thread.joinable()) { m_rootNode->m_thread.join(); }
}

void DirectoryViewDock::OpenFolder(const std::string& path)
{
    LOG("DirectoryViewDock::OpenFolder", path);

    m_currentFolder = path;
    m_rootNode      = DirectoryNode::CreateDirectryNodeTreeFromPathAsync(path);
}

void DirectoryViewDock::Render()
{   
    if (!m_visible)
    {   
        return;
    }

    ImGui::ShowDemoWindow();
        
    ImGui::SetNextWindowClass(&m_windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });

    ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_Border, { 0.3f, 0.3f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ChildBg, COLOR_CHILD_BG_4);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, COLOR_CHILD_BG_4);

    ImGui::Begin("Directory View");
    {
        ImGui::BeginChild("##Background", { -1, -1 }, true);
        {
            ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
            ImGui::SetCursorPos({ 10, 6 });
            ImGui::Text("FOLDERS");
            ImGui::PopFont();

            ImGui::SameLine();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_CHILD_BG_4);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_CHILD_BG_4);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_CHILD_BG_4);

            ImGui::PushStyleColor(ImGuiCol_Text, { 0.3, 0.3, 0.3, 1.0f});

            auto scale = EngineAssetManager::Get()->m_fonts["JetBrains"]->Scale;
            ImGui::SetWindowFontScale(scale/1.5);
            if(ImGui::Button(ICON_FA_FILE_CIRCLE_PLUS, { 28, 23 }))
            {
            }
            ImGui::SameLine();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 75);
            if(ImGui::Button(ICON_FA_FOLDER_PLUS, { 28, 23 }))
            {
            }
            ImGui::SameLine();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 35);
            ImGui::SetWindowFontScale(scale);
            ImGui::PopStyleColor(3);

            if(ImGui::Button(ICON_FA_MINUS, { 28, 23 }))
            {
                m_visible = false;
            }
            ImGui::PopStyleColor();
            ImGui::BeginChild("##DirectoryViewDisplay", {-1, -1});
            if (m_rootNode && !m_currentFolder.empty())
            {
                if(ImGui::BeginTable("test table", 1, ImGuiTableFlags_RowBg))
                {
                    RecursivelyDisplayDirectoryNode(m_rootNode);
                    ImGui::EndTable();
                }
            
                if (m_removeFile)
                {
                    if (m_removeFile == m_rootNode)
                    {
                        m_rootNode = nullptr;

                    }
                    else
                    {
                        auto it = std::find(m_removeFile->m_parent->m_children.begin(), m_removeFile->m_parent->m_children.end(), m_removeFile);
                        m_removeFile->m_parent->m_children.erase(it);
                    }
                    m_removeFile = nullptr;
                }

            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar();
}

DirectoryViewDock::~DirectoryViewDock()
{
    LOG("DirectoryViewDock::~DirectoryViewDock");
}

void DirectoryViewDock::OpenContextMenu(const PTR<DirectoryNode>& path, bool isDirectory)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.1, 0.5 });
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10, 10 });

    ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
    ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.0641, 0.0641, 0.0641, 1 });

    if (ImGui::BeginPopupContextItem())
    { 
        if (isDirectory)
        {
            OpenFolderContextMenu(path);
        }
        else
        {
            OpenFileContextMenu(path);
        }
        ImGui::EndPopup();
    }

    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(3);
}

void DirectoryViewDock::OpenFileContextMenu(const PTR<DirectoryNode>& path)
{    
    bool openDeleteModal = false;
    bool openRenameModal = false;

    if(ImGui::Button("Rename...", { 150, 25 }))
    {
        openRenameModal = true;
    }

    if(ImGui::Button("Delete file", { 150, 25 }))
    {
        openDeleteModal = true;
    }

    if (openDeleteModal)
    {
        ImGui::OpenPopup("Open Delete Modal");
    }
    
    if (openRenameModal)
    {
        ImGui::OpenPopup("Open Rename Modal");
    }
    OpenDeleteModal(path);
    OpenRenameModal(path);
}

void DirectoryViewDock::OpenRenameModal(const PTR<DirectoryNode>& path)
{
    const int modalWidth  = 400;
    const int modalHeight = 120;

    bool unsuedOpen = true;

    ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkSize.x / 2 - modalWidth / 2, ImGui::GetMainViewport()->WorkSize.y / 2 - modalHeight / 2 });
    ImGui::SetNextWindowSize({ modalWidth, modalHeight });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5);
    ImGui::PushStyleColor(ImGuiCol_Border, COLOR_BORDER_4);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });

    if (ImGui::BeginPopupModal("Open Rename Modal", &unsuedOpen, m_modalFlags))
    {
        ImGui::SetCursorPosY(5);
        ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
        ImGui::Text(" RENAME");
        ImGui::PopFont();
        ImGui::SetCursorPosX(modalWidth / 2 - ImGui::CalcTextSize("Enter new name:").x / 2);
        ImGui::Text("Enter new name:");
        
        static char filename[255];
        static bool setName = true;
        if (setName)
        {
            memcpy(filename, path->Name().c_str(), path->Name().size());
            setName = false;
        }
        ImGui::SetCursorPosX(50);
        ImGui::SetNextItemWidth(300);
        ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
        ImGui::InputText("##newfileName", filename, 255);
        ImGui::PopFont();
        ImGui::SetCursorPosX(modalWidth / 2 - 65);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.5, 0.5 });

        ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
        if (ImGui::Button("RENAME", { 60, 30 }))
        {   
            if(path->m_isDirectory)
            {
                auto oldPath = path->Path();       

                path->Rename(std::string(filename));
            }
            else
            {
                LOG("File rename to:", std::string(filename));     
                auto oldPath = path->Path();       
                path->m_file.Rename(std::string(filename));
            
                auto& files = EngineUI::Get()->GetDock<OpenedFilesDock>()->m_files;
                if (files.find(oldPath) != files.end())
                {
                    auto value = files[oldPath]; 
                    
                    files.erase(oldPath);
                    files[path->Path()] = value;
                    value->m_file = path->m_file;
                }
            }
          
            setName = true;
            memset(filename, NULL, sizeof(filename));

            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("CANCEL", { 60, 30}))
        {
            LOG("Cancel delete file!");
            setName = true;
            memset(filename, NULL, sizeof(filename));
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();

        ImGui::PopStyleVar();

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void DirectoryViewDock::OpenDeleteModal(const PTR<DirectoryNode>& path)
{
    const int modalWidth  = 400;
    const int modalHeight = 120;

    bool unsuedOpen = true;

    ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkSize.x / 2 - modalWidth / 2, ImGui::GetMainViewport()->WorkSize.y / 2 - modalHeight / 2 });
    ImGui::SetNextWindowSize({ modalWidth, modalHeight });

    ImGui::PushStyleColor(ImGuiCol_Border, COLOR_BORDER_4);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });

    if (ImGui::BeginPopupModal("Open Delete Modal", &unsuedOpen, m_modalFlags))
    {
        ImGui::SetCursorPosY(5);
        ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
        ImGui::Text(" DELETE");
        ImGui::PopFont();

        ImGui::SetCursorPosX(modalWidth / 2 - ImGui::CalcTextSize(ICON_FA_TRIANGLE_EXCLAMATION).x / 2);
        ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);

        std::string message = std::string("Are you sure that you want to delete ") + ((path->m_isDirectory) ? "folder?" : "file?");

        ImGui::SetCursorPosX(modalWidth / 2 - ImGui::CalcTextSize(message.c_str()).x / 2);
        ImGui::Text(message.c_str());

        ImGui::SetCursorPosX(modalWidth / 2 - 40);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.5, 0.5 });

        ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
        if (ImGui::Button("YES", { 40, 30 }))
        {
            if (path->m_isDirectory)
            {
                if (std::filesystem::remove_all(path->Path()))
                {
                    LOG("Folder deleted:", path->Path());
                    m_removeFile = path;
                }
                else { LOG("Failed to delete folder:", path->Path()); }
            }
            else
            {
                if (std::filesystem::remove(path->Path()))
                {
                    LOG("File deleted:", path->Path());
                    m_removeFile = path;
                }
                else { LOG("Failed to delete file:", path->Path()); }
            }
            
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("NO", { 40, 30 }))
        {
            LOG("Cancel delete file!");
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();
        ImGui::PopStyleVar();

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void DirectoryViewDock::OpenFolderContextMenu(const PTR<DirectoryNode>& path)
{
    const ImVec2 buttonSize = { 150, 25 }; 

    if(ImGui::Button("New File", buttonSize)) 
    { 

    }
    if(ImGui::Button("New Folder...", buttonSize)) 
    { 

    }

    ImGui::Separator();

    if(ImGui::Button("Rename...", buttonSize)) 
    {
        ImGui::OpenPopup("Open Rename Modal"); 
    }
    if(ImGui::Button("Delete Folder", buttonSize)) 
    {
        ImGui::OpenPopup("Open Delete Modal"); 
    }

    OpenDeleteModal(path);
    OpenRenameModal(path);
}