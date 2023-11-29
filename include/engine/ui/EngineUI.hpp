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
#include <engine/ui/EngineAssetManager.hpp>

#include <utils/Types.hpp>

class EngineUI
{
private:
    static inline PTR<EngineUI> s_instance = nullptr;

public:
    static PTR<EngineUI> Get();
    
    void Destroy();

    PTR<EngineAssetManager> m_assetManager;
    
    EngineUI();
   ~EngineUI();

    void OpenFile(const std::string& path, const std::string& fileName, const std::string& ext);

    void Render() ;
    template<typename TDock>
    std::shared_ptr<TDock> GetDock()
    {
        return std::dynamic_pointer_cast<TDock>(m_uiDocks[typeid(TDock)]);
    }

public:
    std::unordered_map<std::type_index, std::shared_ptr<EngineUIDock>> m_uiDocks;
};

#endif