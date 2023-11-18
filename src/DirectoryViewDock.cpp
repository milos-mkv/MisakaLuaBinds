#include <engine/ui/docks/DirectoryViewDock.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <engine/gl/Texture.hpp>

void DirectoryViewDock::RecursivelyAddDirectoryNodes(DirectoryNode& parentNode, std::filesystem::directory_iterator directoryIterator)
{
	for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		DirectoryNode& childNode = parentNode.Children.emplace_back();
		childNode.FullPath = entry.path().c_str();
		childNode.FileName = entry.path().filename().c_str();
		if (childNode.IsDirectory = entry.is_directory(); childNode.IsDirectory)
			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
	}

	auto moveDirectoriesToFront = [](const DirectoryNode& a, const DirectoryNode& b) { return (a.IsDirectory > b.IsDirectory); };
	std::sort(parentNode.Children.begin(), parentNode.Children.end(), moveDirectoriesToFront);
}

DirectoryNode DirectoryViewDock::CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath)
{
	DirectoryNode rootNode;
	rootNode.FullPath = rootPath.c_str();
	rootNode.FileName = rootPath.filename().c_str();
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
		if (ImGui::TreeNodeEx(("  " + parentNode.FileName).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
		{
			for (const DirectoryNode& childNode : parentNode.Children)
			{	
                RecursivelyDisplayDirectoryNode(childNode);
            }

			ImGui::TreePop();
		}
        auto cur2 = ImGui::GetCursorPos();
        ImGui::SetCursorPos(cur);
        ImGui::Image((ImTextureID) (m_icons["Folder"].id), {20, 20}); 
        ImGui::SetCursorPos(cur2);
	}
	else
	{
        auto cur = ImGui::GetCursorPos();
        auto cur2 = cur;
        // cur.x += 30;
        // cur.y += 2;
        // ImGui::SetCursorPos(cur);        
        ImGui::SetCursorPosX(0);
        // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0});
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
        if (ImGui::Button(("###  " + parentNode.FileName).c_str(), ImVec2(-1, 25)))
        {
            LOG(parentNode.FullPath);
        }
        ImGui::PopStyleColor(2);
        ImGui::SameLine();

        cur.x += 30;
        cur.y += 2;
        ImGui::SetCursorPos(cur);

        ImGui::Image((ImTextureID) (m_icons["File"].id), {20, 20}); 
        ImGui::SameLine();
        cur2.y -= 4;
        ImGui::SetCursorPos(cur2);

        ImGui::Text(("     " + parentNode.FileName).c_str());
        // ImGui::PopStyleVar();

	}
	ImGui::PopID();
}

DirectoryViewDock::DirectoryViewDock()
{
    std::filesystem::path sss("/Users/milos.milicevic/Documents/GitHub/MisakaLuaBinds");
    rootNode = CreateDirectryNodeTreeFromPath(sss);

    m_icons.insert(std::make_pair<std::string, Texture>("Folder", Texture::CreateTexture("./folder.png")));
    m_icons.insert(std::make_pair<std::string, Texture>("File", Texture::CreateTexture("./file.png")));
}

void DirectoryViewDock::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Directory View");
    RecursivelyDisplayDirectoryNode(rootNode);
    ImGui::End();
    ImGui::PopStyleVar();
}