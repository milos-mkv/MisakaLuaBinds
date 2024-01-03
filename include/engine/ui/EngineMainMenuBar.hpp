#ifndef MISAKA_ENGINE_MAIN_MENU_BAR
#define MISAKA_ENGINE_MAIN_MENU_BAR

#include <engine/ui/EngineUIDock.hpp>
#include <string>
#include <imgui.h>
class EngineMainMenuBar : public EngineUIDock
{
public:
    bool m_openCreateNewProjectPopup;
    ImGuiWindowFlags m_createProjectModalFlags;

    void Render() override;
    void Destroy() override;

    EngineMainMenuBar();
   ~EngineMainMenuBar() {}
    void OpenCreateNewProjectPopup();

    void MenuActionNewFile();
    void MenuActionOpenFile();
    void MenuActionOpenFolder();
    void MenuActionSaveFile();
    void MenuActionSaveFileAs();
    void MenuActionExit();

    void MenuActionUndoAction();
    void MenuActionFindAction();
    void MenuActionRedoAction();
    void MenuActionCopyAction();
    void MenuActionCutAction();
    void MenuActionPasteAction();

    void MenuActionToggleDirectoryView();

    void MenuActionAbout();


    void CreateNewProjectPopup();

    void OnCreateNewProjectConfirm(const std::string& name, const std::string& path);
};

#endif