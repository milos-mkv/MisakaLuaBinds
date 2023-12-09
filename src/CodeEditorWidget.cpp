#include <engine/ui/widgets/CodeEditorWidget.hpp>

#include <engine/ui/EngineAssetManager.hpp>
#include <engine/ui/docks/OpenedFilesDock.hpp>
#include <engine/ui/EngineUI.hpp>
#include <utils/Logger.hpp>
#include <fstream>
#include <utils/Utils.hpp>
#include <engine/ui/EngineUIActions.hpp>

CodeEditorWidget::CodeEditorWidget(const File& file)
    : m_file(file), m_flags(ImGuiTabBarFlags_None), m_alive(true)
{
    LOG("CodeEditorWidget::CodeEditorWidget", m_file.Path(), m_file.Name(), m_file.Extn());

    m_editor = CreatePTR(TextEditor);
    if (m_file.Extn() == ".lua") 
    {
        m_editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    }

    m_editor->SetText(m_file.Read());
}

CodeEditorWidget::~CodeEditorWidget()
{
    LOG("CodeEditorWidget::~CodeEditorWidget", m_file.Path(), m_file.Name(), m_file.Extn());
}

void CodeEditorWidget::Render()
{
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, COLOR_CHILD_BG_4 );
    ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains24"]);
    m_editor->Render(m_file.Path().c_str(), { -6, -6 }, false);
    ImGui::PopFont();
    ImGui::PopStyleColor();
}

void CodeEditorWidget::Save()
{
    if (m_file.Path().empty())
    {
        auto path = EngineSaveFileDialog();

        if (path.empty())
        {
            LOG("CodeEditorWidget - Canceling save file.");
            return;
        }

        m_file = File(path);
    }

    if (!m_file.Write(m_editor->GetText()))
    {
        LOG("Failed to save file!", m_file.Path());
    }
}

void CodeEditorWidget::SaveAs()
{
    std::string path = EngineSaveFileDialog();

    if (path.empty())
    {
        LOG("CodeEditorWidget - Canceling save file as.");
        return;
    }

    auto dock = EngineUI::Get()->GetDock<OpenedFilesDock>();
    if (!m_file.Path().empty())
    {
        dock->m_files[path] = dock->m_files[m_file.Path()];
        dock->m_files.erase(m_file.Path());
    }

    m_file = File(path);
    
    if (!m_file.Write(m_editor->GetText()))
    {
        LOG("Failed to save file:", m_file.Path());
    }
}