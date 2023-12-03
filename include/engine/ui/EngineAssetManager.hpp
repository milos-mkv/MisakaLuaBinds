#ifndef MISAKA_ENGINE_ASSET_MANAGER
#define MISAKA_ENGINE_ASSET_MANAGER


#include <memory>
#include <engine/gl/Texture.hpp>
#include <unordered_map>
#include <imgui.h>
#include <utils/Types.hpp>
class EngineAssetManager
{
private:
    static inline PTR<EngineAssetManager> s_instance = nullptr;

public:
    static PTR<EngineAssetManager> Get();
    
    void Destroy();

    EngineAssetManager();
   ~EngineAssetManager();
   
    std::unordered_map<std::string, Texture> m_textures;
    std::unordered_map<std::string, ImFont*> m_fonts;
};

#endif