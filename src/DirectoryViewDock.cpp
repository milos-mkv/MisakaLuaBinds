#include <engine/ui/docks/DirectoryViewDock.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <FontAwesomeIcons.hpp>
#include <engine/gl/Texture.hpp>
#include <engine/ui/EngineUI.hpp>
#include <utils/Logger.hpp>

void DirectoryViewDock::RecursivelyAddDirectoryNodes(DirectoryNode& parentNode, std::filesystem::directory_iterator directoryIterator)
{
	for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		DirectoryNode& childNode = parentNode.Children.emplace_back();
		childNode.FullPath  = entry.path().c_str();
		childNode.FileName  = entry.path().filename().c_str();
        childNode.Extension = entry.path().extension().c_str();

		if (childNode.IsDirectory = entry.is_directory(); childNode.IsDirectory)
			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
	}
    
    std::sort(parentNode.Children.begin(), parentNode.Children.end(),[](DirectoryNode& d1, DirectoryNode& d2){
        return d1.FileName[0] < d2.FileName[0];
    });

	auto moveDirectoriesToFront = [](const DirectoryNode& a, const DirectoryNode& b) { return (a.IsDirectory > b.IsDirectory); };
	std::sort(parentNode.Children.begin(), parentNode.Children.end(), moveDirectoriesToFront);
}

DirectoryNode DirectoryViewDock::CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath)
{
	DirectoryNode rootNode;
	rootNode.FullPath  = rootPath.c_str();
	rootNode.FileName  = rootPath.filename().c_str();
    rootNode.Extension = rootPath.extension().c_str();

	if (rootNode.IsDirectory = std::filesystem::is_directory(rootPath); rootNode.IsDirectory)
		RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

	return rootNode;
}



void DirectoryViewDock::RecursivelyDisplayDirectoryNode(const DirectoryNode& parentNode)
{
	ImGui::PushID(&parentNode);
	if (parentNode.IsDirectory)
	{
        auto cur = ImGui::GetCursorPos();
        cur.x += 30;
        cur.y += 2;
		if (ImGui::TreeNodeEx(("       " + parentNode.FileName).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
		{
            OpenContextMenu(parentNode.FullPath.c_str(), true);

			for (const DirectoryNode& childNode : parentNode.Children)
			{	
                RecursivelyDisplayDirectoryNode(childNode);
            }
			ImGui::TreePop();
		}
        else
        {
            OpenContextMenu(parentNode.FullPath.c_str(), true);
        }
        

        auto cur2 = ImGui::GetCursorPos();
        cur.y -= 2;
        ImGui::SetCursorPos(cur);
        ImGui::Image((ImTextureID) (m_icons["Folder"].id), {20, 20}); 
        ImGui::SetCursorPos(cur2);
	}
	else
	{
        auto cur = ImGui::GetCursorPos();
        auto cur2 = cur;    
        ImGui::SetCursorPosX(0);
        if (selectedFile == parentNode.FullPath)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        }
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
        if (ImGui::Button(("###  " + parentNode.FileName).c_str(), ImVec2(-1, 25)))
        {
            LOG("File selected:", parentNode.FullPath);
            selectedFile = parentNode.FullPath;
        }
        OpenContextMenu(parentNode.FullPath.c_str(), false);
        ImGui::PopStyleColor(2);
        ImGui::SameLine();

        cur.x += 32;
        cur.y += 6;
        ImGui::SetCursorPos(cur);

        ImGui::Image((ImTextureID) (m_icons[ parentNode.Extension == ".lua" ? "Lua" : "File"].id), {16, 16}); 
        ImGui::SameLine();
        ImGui::SetCursorPos(cur2);
        ImGui::Text(("             "  + parentNode.FileName).c_str());
	}
	ImGui::PopID();
}

DirectoryViewDock::DirectoryViewDock()
{
    m_icons.insert(std::make_pair<std::string, Texture>("Folder", Texture::CreateTexture("./folder.png")));
    m_icons.insert(std::make_pair<std::string, Texture>("File", Texture::CreateTexture("./file.png")));
    m_icons.insert(std::make_pair<std::string, Texture>("Lua", Texture::CreateTexture("./lua.png")));
}

void DirectoryViewDock::Destroy()
{
    Texture::DestroyTexture(m_icons["Folder"]);
    Texture::DestroyTexture(m_icons["File"]);
    Texture::DestroyTexture(m_icons["Lua"]);
}

void DirectoryViewDock::OpenFolder(const char* path)
{
    currentFolder = path;
    LOG(currentFolder);

    std::filesystem::path sss(path);
    rootNode = CreateDirectryNodeTreeFromPath(sss);
}

void DirectoryViewDock::Render()
{   
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = 1 << 12 ;
    ImGui::SetNextWindowClass(&window_class);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    bool a = true;
    ImGui::Begin("Directory View", &a);
    ImGui::Separator();
    ImGui::PushFont(EngineUI::Get()->font);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5f, 1.0f));
    ImGui::Text(" FOLDERS");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    if (!currentFolder.empty())
    {
        RecursivelyDisplayDirectoryNode(rootNode);
    }
    ImGui::End();
    ImGui::PopStyleVar();

}

DirectoryViewDock::~DirectoryViewDock()
{
}

void DirectoryViewDock::OpenContextMenu(const char* path, bool isDirectory)
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

void DirectoryViewDock::OpenFileContextMenu(const char* path)
{    
    if(ImGui::Button("Rename...", { 150, 25 }))
    {
    }
    if(ImGui::Button("Delete file", { 150, 25 }))
    {

    }
}

void DirectoryViewDock::OpenFolderContextMenu(const char* path)
{
    if(ImGui::Button("New File", { 150, 25})) {}
    if(ImGui::Button("Rename...", { 150, 25})){}
    ImGui::Separator();
    if(ImGui::Button("New Folder...", { 150, 25})){}
    if(ImGui::Button("Delete Folder", { 150, 25})){}
}