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

void DirectoryViewDock::RecursivelyAddDirectoryNodes(const PTR<DirectoryNode>& parentNode, std::filesystem::directory_iterator directoryIterator)
{
	for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		PTR<DirectoryNode> childNode = CreatePTR(DirectoryNode);
		childNode->FullPath  = entry.path().c_str();
		childNode->FileName  = entry.path().filename().c_str();
        childNode->Extension = entry.path().extension().c_str();

        parentNode->Children.push_back(childNode);
		if (childNode->IsDirectory = entry.is_directory(); childNode->IsDirectory)
			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
	}
    
    std::sort(parentNode->Children.begin(), parentNode->Children.end(),[](const PTR<DirectoryNode>& d1, const PTR<DirectoryNode>& d2){
        return d1->FileName[0] < d2->FileName[0];
    });

	auto moveDirectoriesToFront = [](const PTR<DirectoryNode>& a, const PTR<DirectoryNode>& b) { return (a->IsDirectory > b->IsDirectory); };
	std::sort(parentNode->Children.begin(), parentNode->Children.end(), moveDirectoriesToFront);
}

PTR<DirectoryNode> DirectoryViewDock::CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath)
{
	PTR<DirectoryNode> rootNode = CreatePTR(DirectoryNode);
	rootNode->FullPath  = rootPath.c_str();
	rootNode->FileName  = rootPath.filename().c_str();
    rootNode->Extension = rootPath.extension().c_str();

	if (rootNode->IsDirectory = std::filesystem::is_directory(rootPath); rootNode->IsDirectory)
    {
        if(m_directoryLoaderThread.joinable())
        {    
            m_directoryLoaderThread.join();
        }
		m_directoryLoaderThread = std::thread(&DirectoryViewDock::RecursivelyAddDirectoryNodes, this, rootNode, std::filesystem::directory_iterator(rootPath));
    }
	return rootNode;
}

void DirectoryViewDock::RecursivelyDisplayDirectoryNode(const PTR<DirectoryNode>& parentNode)
{
	ImGui::PushID(&parentNode);
	if (parentNode->IsDirectory)
	{
		if (ImGui::TreeNodeEx((" " ICON_FA_FOLDER " " + parentNode->FileName).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
		{
            OpenContextMenu(parentNode->FullPath.c_str(), true);

			for (const auto& childNode : parentNode->Children)
			{	
                RecursivelyDisplayDirectoryNode(childNode);
            }
			ImGui::TreePop();
		}
        else
        {
            OpenContextMenu(parentNode->FullPath.c_str(), true);
        }
	}
	else
	{
        auto cur = ImGui::GetCursorPos();
        auto cur2 = cur;    
        ImGui::SetCursorPosX(0);
        if (m_selectedFile == parentNode->FullPath)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        }
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
        if (ImGui::Button(("###  " + parentNode->FileName).c_str(), ImVec2(-1, 25)))
        {
            LOG("File selected:", parentNode->FullPath);
            m_selectedFile = parentNode->FullPath;
            EngineUI::Get()->GetDock<OpenedFilesDock>()->OpenFile(parentNode->FullPath, parentNode->FileName, parentNode->Extension);
        }
        OpenContextMenu(parentNode->FullPath.c_str(), false);
        ImGui::PopStyleColor(2);
        ImGui::SameLine();

        cur.x += 32;
        cur.y += 6;
        ImGui::SetCursorPos(cur);

        ImGui::SameLine();
        ImGui::SetCursorPos(cur2);
        ImGui::Text(("        " ICON_FA_FILE_CODE "  "  + parentNode->FileName).c_str());
	}
	ImGui::PopID();
}

DirectoryViewDock::DirectoryViewDock()
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
    if (m_directoryLoaderThread.joinable())
    {
        m_directoryLoaderThread.join();
    }
}

void DirectoryViewDock::OpenFolder(const std::string& path)
{
    LOG("DirectoryViewDock::OpenFolder", path);
    m_currentFolder = path;
    m_rootNode = CreateDirectryNodeTreeFromPath(std::filesystem::path(path));
}

void DirectoryViewDock::Render()
{   
    if (!m_visible)
    {   
        return;
    }
        
    ImGui::SetNextWindowClass(&m_windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.0f });

    ImGui::Begin("Directory View");

    ImGui::PushFont(EngineAssetManager::Get()->fonts["JetBrains"]);
    ImGui::Text(" FOLDERS");
    ImGui::PopFont();

    if (!m_currentFolder.empty())
    {
        RecursivelyDisplayDirectoryNode(m_rootNode);
    }

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

DirectoryViewDock::~DirectoryViewDock()
{
    LOG("DirectoryViewDock::~DirectoryViewDock");
}

void DirectoryViewDock::OpenContextMenu(const std::string& path, bool isDirectory)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.1, 0.5 });
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10, 10 });
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1450980392156863,0.1607843137254902,0.2, 1));

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

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

void DirectoryViewDock::OpenFileContextMenu(const std::string& path)
{    
    if(ImGui::Button("Rename...", { 150, 25 }))
    {

    }
    if(ImGui::Button("Delete file", { 150, 25 }))
    {

    }
}

void DirectoryViewDock::OpenFolderContextMenu(const std::string& path)
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

    }
}