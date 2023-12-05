#ifndef MISAKA_ENGINE_OPENED_FILES_DOCK
#define MISAKA_ENGINE_OPENED_FILES_DOCK

#include <engine/ui/EngineUIDock.hpp>
#include <engine/ui/widgets/CodeEditorWidget.hpp>
#include <unordered_map>
#include <utils/FileSystemUtils.hpp>
#include <string>
#include <utils/Types.hpp>
#include <imgui.h>
#include <stack>

class OpenedFilesDock : public EngineUIDock
{
public:
    std::unordered_map<std::string, PTR<CodeEditorWidget>> m_files;
    std::string m_selected;
    ImGuiWindowClass m_windowClass; 
    ImGuiTabBarFlags m_tabBarFlags;
    std::stack<std::string> m_tabsToRemove;

public:
    OpenedFilesDock();
   ~OpenedFilesDock();

    virtual void Render()  override;
    virtual void Destroy() override;

    void OpenFile(const File& file);
    void OpenEmptyFile();
};

#endif