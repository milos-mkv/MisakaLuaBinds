        // ImGui::Begin("Finder");
        // 	RecursivelyDisplayDirectoryNode(rootNode);
        // ImGui::End();
	    // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::ColorConvertU32ToFloat4(editor.GetPalette()[(int)TextEditor::PaletteIndex::Background]));
        // ImGui::Begin("script.lua");

        // ImGui::PushFont(pFont);
		// editor.Render("TextEditor");


        // ImGui::PopStyleColor();

        // ImGui::PopFont();
        // ImGui::End();

#ifndef MISAKA_ENGINE_UI
#define MISAKA_ENGINE_UI

#include <memory>
#include <unordered_map>
#include <string>
#include "EngineUIDockComponent.hpp"
#include <engine/components/RenderableComponent.hpp>

class EngineUI : public RenderableComponent
{
private: 
    static inline std::shared_ptr<EngineUI> s_instance = nullptr;

public:
    static std::shared_ptr<EngineUI> Get();

    EngineUI(); /* CTOR */
   ~EngineUI(); /* DTOR */

    virtual void Render() override;

private:
    std::unordered_map<std::string, std::shared_ptr<EngineUIDockComponent>> m_uiDocks;


};

#endif