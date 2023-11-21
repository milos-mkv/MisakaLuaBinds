#include <engine/ui/docks/CodeEditorDock.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <engine/ui/EngineUI.hpp>
#include <imgui_internal.h>
#include <fstream>
#include <engine/Engine.hpp>

#include <utils/Logger.hpp>

CodeEditorDock::CodeEditorDock(const std::string& path)
    : path(path)
{
    LOG("CodeEditorDock::CodeEditorDock");
    editor = new TextEditor();
	auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    editor->SetLanguageDefinition(lang);

	std::ifstream file(path);
    if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		editor->SetText(str);
        file.close();
	}
}

int a = true;
void CodeEditorDock::Render()
{

    ImGui::SetNextWindowDockID(Engine::DID, ImGuiCond_Once);
    ImGui::PushID(path.c_str());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::Begin(path.c_str());
    
    ImGui::PushFont(EngineUI::Get()->font);

    editor->Render(path.c_str());
    ImGui::PopFont();
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopID();
}
