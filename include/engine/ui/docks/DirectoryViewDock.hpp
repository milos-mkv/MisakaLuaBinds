#ifndef MISAKA_ENGINE_DIRECTORY_VIEW_DOCK
#define MISAKA_ENGINE_DIRECTORY_VIEW_DOCK

#include "../EngineUIDockComponent.hpp"
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
	std::vector<DirectoryNode> Children;
	bool IsDirectory;
};

class DirectoryViewDock : public EngineUIDockComponent
{
public:

    virtual void Render() override;
    DirectoryViewDock();
   ~DirectoryViewDock() { LOG("DESTROY"); } 

private:
    DirectoryNode rootNode;
    std::unordered_map<std::string, Texture> m_icons;
    
    void RecursivelyAddDirectoryNodes(DirectoryNode& parentNode, std::filesystem::directory_iterator directoryIterator);
    DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath);
    void RecursivelyDisplayDirectoryNode(const DirectoryNode& parentNode);
};

#endif