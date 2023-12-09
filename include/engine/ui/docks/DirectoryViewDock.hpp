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
#include <utils/FileSystemUtils.hpp>
#include <imgui.h>
#include <imgui_internal.h>

class DirectoryViewDock : public EngineUIDock
{
public:
    std::string m_currentFolder;
    std::string m_selectedFile;
    PTR<DirectoryNode> m_rootNode;
    ImGuiWindowClass m_windowClass;
    ImGuiWindowFlags m_modalFlags;
    bool m_visible;
    PTR<DirectoryNode> m_removeFile;

public:
    DirectoryViewDock();
   ~DirectoryViewDock();

    void Render()  override;
    void Destroy() override;

    void OpenFolder(const std::string& path);

public:
    void OpenContextMenu(const PTR<DirectoryNode>& path, bool isDirectory);
    void OpenFileContextMenu(const PTR<DirectoryNode>& id);
    void OpenFolderContextMenu(const PTR<DirectoryNode>& id);
    
    ImRect RecursivelyDisplayDirectoryNode(const PTR<DirectoryNode>& parentNode);
    void OpenDeleteModal(const PTR<DirectoryNode>& path);
    void OpenRenameModal(const PTR<DirectoryNode>& path);
};

#endif