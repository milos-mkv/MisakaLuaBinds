#ifndef MISAKA_ENGINE_MAIN_MENU_BAR
#define MISAKA_ENGINE_MAIN_MENU_BAR

#include <engine/ui/EngineUIDock.hpp>
#include <string>

class EngineMainMenuBar : public EngineUIDock
{
public:
    bool openCreateNewProjectPopup = false;

    void Render() override;
    void Destroy() override;

    EngineMainMenuBar();
   ~EngineMainMenuBar() {}

    void NewFile();
    void OpenFile();
    void OpenFolder();
    void SaveFile();
    void SaveFileAs();
    void Exit();
    void UndoAction();
    void FindAction();
    void RedoAction();
    void CopyAction();
    void CutAction();
    void PasteAction();
    void OpenCreateNewProjectPopup();

    void CreateNewProjectPopup();

    void OnCreateNewProjectConfirm(const std::string& name, const std::string& path, const std::string& type);
};

#endif