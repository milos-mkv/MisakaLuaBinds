#ifndef MISAKA_ENGINE_CODE_EDITOR_WIDGET
#define MISAKA_ENGINE_CODE_EDITOR_WIDGET

#include <string>
#include <texteditor/TextEditor.h>
#include <utils/Types.hpp>
#include <imgui.h>

class CodeEditorWidget
{
public:
    std::string m_path;
    std::string m_fileName;
    std::string m_ext;
    PTR<TextEditor> m_editor;
    ImGuiTabItemFlags m_flags;
    bool m_alive;

    CodeEditorWidget(const std::string& path, const std::string& fileName, const std::string& ext);
   ~CodeEditorWidget();

    void Render();
    void Save();
    void SaveAs();
};

#endif