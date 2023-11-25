#include <engine/ui/docks/CodeEditorDock.hpp>
#include <engine/ui/EngineAssetManager.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <engine/ui/EngineUI.hpp>
#include <imgui_internal.h>
#include <string>
#include <fstream>
#include <engine/Engine.hpp>
#include <utils/Logger.hpp>

CodeEditorDock::CodeEditorDock(const std::string& path, const std::string& fileName, const std::string& ext)
    : path(path), fileName(fileName), ext(ext)
{
    LOG("CodeEditorDock::CodeEditorDock");
    editor = new TextEditor();
    if (ext == ".lua") 
    {
        auto lang = TextEditor::LanguageDefinition::Lua();
        editor->SetLanguageDefinition(lang);
    }
    else if (ext == ".cpp" || ext == ".cc") 
    {
        auto lang = TextEditor::LanguageDefinition::CPlusPlus();
        editor->SetLanguageDefinition(lang);
    }
    else if (ext == ".c") 
    {
        auto lang = TextEditor::LanguageDefinition::C();
        editor->SetLanguageDefinition(lang);
    }

	std::ifstream file(path);
    if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		editor->SetText(str);
        file.close();
	}

    LOG("ASDAS");
}

void CodeEditorDock::Render()
{
    if (!alive)
    {
        return;
    }
    
    // ImGui::SetNextWindowDockID(Engine::DID, ImGuiCond_Once);

    // ImGui::PushID(path.c_str());
    // ImGuiWindowClass window_class;
    // window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoCloseButton |
    // ImGuiDockNodeFlags_NoWindowMenuButton ;
    
    // ImGui::SetNextWindowClass(&window_class);
        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImGui::ColorConvertU32ToFloat4(4279242768));

    ImGui::PushFont(EngineAssetManager::Get()->fonts["JetBrains"]);
    // ImGui::Begin(path.c_str(), &alive, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_UnsavedDocument);

    editor->Render(path.c_str());
    // ImGui::End();
    // auto cor = editor->GetCursorPosition();
    // LOG(cor.mLine, cor.mColumn);
    // ImGui::Text(std::to_string(cor.mColumn).c_str());
    ImGui::PopFont();

    ImGui::PopStyleColor();
    // ImGui::PopID();
    
}
