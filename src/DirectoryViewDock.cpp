#include <engine/ui/docks/DirectoryViewDock.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

struct DirectoryNode
{
	std::string FullPath;
	std::string FileName;
	std::vector<DirectoryNode> Children;
	bool IsDirectory;
};

static void RecursivelyAddDirectoryNodes(DirectoryNode& parentNode, std::filesystem::directory_iterator directoryIterator)
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

static DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath)
{
	DirectoryNode rootNode;
	rootNode.FullPath = rootPath.c_str();
	rootNode.FileName = rootPath.filename().c_str();
	if (rootNode.IsDirectory = std::filesystem::is_directory(rootPath); rootNode.IsDirectory)
		RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

	return rootNode;
}

static void RecursivelyDisplayDirectoryNode(const DirectoryNode& parentNode)
{
	ImGui::PushID(&parentNode);
	if (parentNode.IsDirectory)
	{
        // auto cur = ImGui::GetCursorPos();
        // cur.x += 4;
        // cur.y += 2;
		if (ImGui::TreeNodeEx(parentNode.FileName.c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
		{
			for (const DirectoryNode& childNode : parentNode.Children)
			{	
                RecursivelyDisplayDirectoryNode(childNode);
            }

			ImGui::TreePop();
		}
        // auto cur2 = ImGui::GetCursorPos();
        // ImGui::SetCursorPos(cur);
        // ImGui::Image((ImTextureID) (id1.id), {20, 20}); 
        // ImGui::SetCursorPos(cur2);
	}
	else
	{
        // auto cur = ImGui::GetCursorPos();
        // auto cur2 = cur;
        // cur.x += 4;
        // cur.y += 2;
        // ImGui::SetCursorPos(cur);

        // ImGui::Image((ImTextureID) (id2.id), {20, 20}); 
        // ImGui::SameLine();
        // ImGui::SetCursorPos(cur2);
		if (ImGui::TreeNodeEx(parentNode.FileName.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth))
		{
		}

	}
	ImGui::PopID();
}
DirectoryNode rootNode;
DirectoryViewDock::DirectoryViewDock()
{
    std::filesystem::path sss("/Users/milos.milicevic/Documents/GitHub/MisakaLuaBinds");
    rootNode = CreateDirectryNodeTreeFromPath(sss);
}

void DirectoryViewDock::Render()
{
    ImGui::Begin("Directory View");
    RecursivelyDisplayDirectoryNode(rootNode);
    ImGui::End();
}