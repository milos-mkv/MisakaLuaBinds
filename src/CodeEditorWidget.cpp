#include <engine/ui/widgets/CodeEditorWidget.hpp>

#include <engine/ui/EngineAssetManager.hpp>
#include <engine/ui/docks/OpenedFilesDock.hpp>
#include <engine/ui/EngineUI.hpp>
#include <utils/Logger.hpp>
#include <fstream>
#include <utils/Utils.hpp>
#include <engine/ui/EngineUIActions.hpp>

CodeEditorWidget::CodeEditorWidget(const std::string& path, const std::string& fileName, const std::string& ext)
    : m_path(path), m_fileName(fileName), m_ext(ext), m_flags(ImGuiTabBarFlags_None), m_alive(true)
{
    LOG("CodeEditorWidget::CodeEditorWidget", path, fileName, ext);

    m_editor = CreatePTR(TextEditor);
    if (m_ext == ".lua") 
    {
        auto lang = TextEditor::LanguageDefinition::Lua();
        m_editor->SetLanguageDefinition(lang);
    }

    std::ifstream file(m_path);
    if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		m_editor->SetText(str);
        file.close();
	}
    else
    {
        LOG("Failed to open file:", m_path);
    }
}

CodeEditorWidget::~CodeEditorWidget()
{
    LOG("CodeEditorWidget::~CodeEditorWidget", m_path, m_fileName, m_ext);
}

void CodeEditorWidget::Render()
{
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImGui::ColorConvertU32ToFloat4(4279242768));
    ImGui::PushFont(EngineAssetManager::Get()->m_fonts["JetBrains"]);
    m_editor->Render(m_path.c_str(), {-6, -6}, false);
    ImGui::PopFont();
    ImGui::PopStyleColor();
}

void CodeEditorWidget::Save()
{
    if (m_path.empty())
    {
        m_path = EngineSaveFileDialog();

        if (m_path.empty())
        {
            LOG("CodeEditorWidget - Canceling save file.");
            return;
        }

    
        auto split = SplitString(m_path, "/");
        m_fileName = split[split.size() - 1];

        auto split1 = SplitString(m_path, ".");
        m_ext = "." + split1[split1.size() - 1];
    }

    std::ofstream file(m_path);
    if (file.good())
	{
        LOG("File saved:", m_path);
		std::string text = m_editor->GetText();
        file << text;
        file.close();
	}
    else
    {
        LOG("Failed to open file:", m_path);
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
    
    dock->m_files[path] = dock->m_files[m_path];
    dock->m_files.erase(m_path);

    m_path = path;

    auto split = SplitString(m_path, "/");
    m_fileName = split[split.size() - 1];

    auto split1 = SplitString(m_path, ".");
    m_ext = "." + split1[split1.size() - 1];
    
    std::ofstream file(m_path);
    if (file.good())
	{
        LOG("File saved:", m_path);
		std::string text = m_editor->GetText();
        file << text;
        file.close();
	}
    else
    {
        LOG("Failed to open file:", m_path);
    }
}