#ifndef MISAKA_ENGINE_ASSET_MANAGER
#define MISAKA_ENGINE_ASSET_MANAGER


#include <memory>
#include <engine/gl/Texture.hpp>
#include <unordered_map>
#include <imgui.h>

class EngineAssetManager
{
private:
    static inline std::shared_ptr<EngineAssetManager> s_instance = nullptr;

public:
    static std::shared_ptr<EngineAssetManager> Get();
     void Destroy();

    EngineAssetManager();
   ~EngineAssetManager();
    std::unordered_map<std::string, Texture> textures;
    std::unordered_map<std::string, ImFont*> fonts;
};

#endif