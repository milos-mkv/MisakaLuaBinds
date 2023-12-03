#ifndef MISAKA_ENGINE_UI
#define MISAKA_ENGINE_UI

#include <unordered_map>
#include <string>
#include "EngineUIDock.hpp"
#include <imgui.h>
#include <typeindex>
#include <engine/ui/docks/CodeEditorDock.hpp>
#include <engine/ui/EngineAssetManager.hpp>

#include <utils/Types.hpp>

class EngineUI
{
private:
    static inline PTR<EngineUI> s_instance = nullptr;

public:
    static PTR<EngineUI> Get();
    
    PTR<EngineAssetManager> m_assetManager;
    
    EngineUI();
   ~EngineUI();

    void Render();
    void Destroy();

    template<typename TDock>
    PTR<TDock> GetDock()
    {
        return std::dynamic_pointer_cast<TDock>(m_uiDocks[typeid(TDock)]);
    }

public:
    std::unordered_map<std::type_index, PTR<EngineUIDock>> m_uiDocks;
};

#endif