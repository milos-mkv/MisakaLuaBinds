#ifndef MISAKA_ENGINE_DIRECTORY_VIEW_DOCK
#define MISAKA_ENGINE_DIRECTORY_VIEW_DOCK

#include "../EngineUIDock.hpp"
#include <utils/Logger.hpp>
#include <unordered_map>
#include <engine/gl/Texture.hpp>
#include <vector>
#include <filesystem>
#include <fstream>


struct DirectoryNode
{
	std::string FullPath;
	std::string FileName;
    std::string Extension;

	std::vector<DirectoryNode> Children;
	bool IsDirectory;
};

class DirectoryViewDock : public EngineUIDock
{
public:

    virtual void Render() override;
    virtual void Destroy() override;
    DirectoryViewDock();
   ~DirectoryViewDock();

    void OpenFolder(const char* path);

private:
    std::string currentFolder;

    DirectoryNode rootNode;
    std::unordered_map<std::string, Texture> m_icons;
    std::string selectedFile;

    void OpenContextMenu(const char* path, bool isDirectory);
    void OpenFileContextMenu(const char* id);
    void OpenFolderContextMenu(const char* id);
    
    void RecursivelyAddDirectoryNodes(DirectoryNode& parentNode, std::filesystem::directory_iterator directoryIterator);
    DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath);
    void RecursivelyDisplayDirectoryNode(const DirectoryNode& parentNode);
};

#endif