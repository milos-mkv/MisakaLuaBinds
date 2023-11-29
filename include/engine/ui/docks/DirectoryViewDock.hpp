#ifndef MISAKA_ENGINE_DIRECTORY_VIEW_DOCK
#define MISAKA_ENGINE_DIRECTORY_VIEW_DOCK

#include <engine/ui/EngineUIDock.hpp>
#include <utils/Logger.hpp>
#include <unordered_map>
#include <engine/gl/Texture.hpp>
#include <vector>
#include <imgui.h>
#include <filesystem>
#include <fstream>
#include <atomic>
#include <thread>
#include <utils/Types.hpp>

struct DirectoryNode
{
	std::string FullPath;
	std::string FileName;
    std::string Extension;

	std::vector<std::shared_ptr<DirectoryNode>> Children;
	bool IsDirectory;
};

class DirectoryViewDock : public EngineUIDock
{
public:
    std::string m_currentFolder;
    std::string m_selectedFile;
    std::thread m_directoryLoaderThread;
    PTR<DirectoryNode> m_rootNode;
    ImGuiWindowClass m_windowClass;
    bool m_visible = true;

public:
    DirectoryViewDock();
   ~DirectoryViewDock();

    void Render()  override;
    void Destroy() override;

    void OpenFolder(const std::string& path);

public:

    void OpenContextMenu(const std::string& path, bool isDirectory);
    void OpenFileContextMenu(const std::string& id);
    void OpenFolderContextMenu(const std::string& id);
    
    void RecursivelyAddDirectoryNodes(const PTR<DirectoryNode>& parentNode, std::filesystem::directory_iterator directoryIterator);
    PTR<DirectoryNode> CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath);
    void RecursivelyDisplayDirectoryNode(const PTR<DirectoryNode>& parentNode);
};

#endif