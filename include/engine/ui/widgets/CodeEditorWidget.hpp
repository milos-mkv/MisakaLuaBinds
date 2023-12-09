#ifndef MISAKA_ENGINE_CODE_EDITOR_WIDGET
#define MISAKA_ENGINE_CODE_EDITOR_WIDGET

#include <string>
#include <texteditor/TextEditor.h>
#include <utils/Types.hpp>
#include <utils/FileSystemUtils.hpp>
#include <imgui.h>

class CodeEditorWidget
{
public:
    File m_file;
    PTR<TextEditor> m_editor;
    ImGuiTabItemFlags m_flags;
    bool m_alive;

    CodeEditorWidget(const File& file);
   ~CodeEditorWidget();

    void Render();
    void Save();
    void SaveAs();
};

#endif