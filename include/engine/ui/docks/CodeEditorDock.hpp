#ifndef MISAKA_ENGINE_CODE_EDITOR_DOCK
#define MISAKA_ENGINE_CODE_EDITOR_DOCK

#include <engine/ui/EngineUIDock.hpp>
#include <string>
#include <memory>
#include <TextEditor.h>

class CodeEditorDock : public EngineUIDock
{
public:
    std::string path;
    std::string fileName;
    std::string ext;
    bool alive = true;
    TextEditor* editor;

    ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;


    CodeEditorDock(const std::string& path, const std::string& fileName = "", const std::string& ext = "");
    void Render() override;
    void Destroy() override { if(editor != nullptr) delete editor; }

};

#endif