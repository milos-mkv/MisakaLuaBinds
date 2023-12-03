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

void DirectoryViewDock::RecursivelyDisplayDirectoryNode(const PTR<DirectoryNode>& parentNode)
{
	ImGui::PushID(&parentNode);
	if (parentNode->m_isDirectory)
	{
		if (ImGui::TreeNodeEx((" " ICON_FA_FOLDER " " + parentNode->Name()).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
		{
            OpenContextMenu(parentNode, true);

			for (const auto& childNode : parentNode->m_children)
			{	
                RecursivelyDisplayDirectoryNode(childNode);
            }
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

            // TODO: Change params.
            EngineUI::Get()->GetDock<OpenedFilesDock>()->OpenFile(parentNode->Path(), parentNode->Name(), parentNode->Extn());
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        OpenContextMenu(parentNode, false);

        ImGui::SameLine();
        ImGui::SetCursorPos(startCursor);
        ImGui::Text(("        " ICON_FA_FILE_CODE "  "  + parentNode->Name()).c_str());
	}
	ImGui::PopID();
}

DirectoryViewDock::DirectoryViewDock() 
    : m_visible(true), m_removeFile(nullptr)
{
    LOG("DirectoryViewDock::DirectoryViewDock");

     m_windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe 
                                            | ImGuiDockNodeFlags_NoDockingSplit
                                            | ImGuiDockNodeFlags_NoCloseButton
                                            | ImGuiDockNodeFlags_NoTabBar;
    OpenFolder("..");
}

void DirectoryViewDock::Destroy()
{
    LOG("DirectoryViewDock::Destroy");
    if (m_rootNode->m_thread.joinable()) { m_rootNode->m_thread.join(); }
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
        
    ImGui::SetNextWindowClass(&m_windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1, 1 });
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.5);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

    ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_Border, { 0.3, 0.3, 0.3, 1});
    ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.0641, 0.0641, 0.0641, 1.00f });
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, { 0.0641, 0.0641, 0.0641, 1.00f });

    ImGui::Begin("Directory View");
    {
        ImGui::BeginChild("##DirectoryViewChild", {-1, -1}, true);
        {
            ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
            ImGui::SetCursorPos({ 10, 6 });
            ImGui::Text("FOLDERS");
            ImGui::PopFont();

            if (m_rootNode && !m_currentFolder.empty())
            {
                RecursivelyDisplayDirectoryNode(m_rootNode);

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
        }
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(4);
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


    if(ImGui::Button("Rename...", { 150, 25 }))
    {
    }

    if(ImGui::Button("Delete file", { 150, 25 }))
    {
        if (std::filesystem::remove(path->Path()))
        {
            LOG("File deleted:", path->Path());
            m_removeFile = path;
        }
        else
        {
            LOG("Failed to delete file:", path->Path());
        }

        ImGui::CloseCurrentPopup();
    }
}

void DirectoryViewDock::OpenFolderContextMenu(const PTR<DirectoryNode>& path)
{
    if(ImGui::Button("New File", { 150, 25})) 
    {

    }
    if(ImGui::Button("Rename...", { 150, 25}))
    {

    }
    ImGui::Separator();
    if(ImGui::Button("New Folder...", { 150, 25}))
    {

    }
    if(ImGui::Button("Delete Folder", { 150, 25}))
    {
        if (std::filesystem::remove_all(path->Path()))
        {
            LOG("Folder deleted:", path->Path());
            m_removeFile = path;
        }
        else
        {
            LOG("Failed to delete Folder:", path->Path());
        }
        ImGui::CloseCurrentPopup();
    }
}