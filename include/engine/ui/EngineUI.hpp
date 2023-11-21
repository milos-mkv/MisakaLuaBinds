#ifndef MISAKA_ENGINE_UI
#define MISAKA_ENGINE_UI

#include <memory>
#include <unordered_map>
#include <string>
#include "EngineUIDock.hpp"
#include <imgui.h>
#include <typeindex>
#include <engine/components/RenderableComponent.hpp>
#include <engine/ui/docks/CodeEditorDock.hpp>

class EngineUI
{
private: 
    static inline std::shared_ptr<EngineUI> s_instance = nullptr;

public:
    static std::shared_ptr<EngineUI> Get();
    static void Destroy();
    ImFont* font;

    EngineUI(); /* CTOR */
   ~EngineUI(); /* DTOR */

    void OpenFile(const std::string& path);

    void Render() ;
    template<typename TDock>
    std::shared_ptr<TDock> GetDock()
    {
        return std::dynamic_pointer_cast<TDock>(m_uiDocks[typeid(TDock)]);
    }

public:
    std::unordered_map<std::type_index, std::shared_ptr<EngineUIDock>> m_uiDocks;
    std::unordered_map<std::string, std::shared_ptr<CodeEditorDock>> m_codeEditors;


};

#endif