#include <engine/ui/EngineAssetManager.hpp>

#include <utils/Logger.hpp>
#include <FontAwesomeIcons.hpp>

std::shared_ptr<EngineAssetManager> EngineAssetManager::Get()
{
    if (s_instance == nullptr)
    {
        s_instance = std::make_shared<EngineAssetManager>();
    }
    return s_instance;
}

ImFontConfig config;
ImFontConfig config1;

EngineAssetManager::EngineAssetManager()
{
    textures["File Icon"] = Texture::CreateTexture("file.png");
    textures["Folder Icon"] = Texture::CreateTexture("folder.png");
    textures["Project"] = Texture::CreateTexture("project.png");


    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImFont* font = io.Fonts->AddFontDefault();
    config.OversampleH = 4;
    config.OversampleV = 4;
    config.PixelSnapH = false;
    fonts["JetBrains"] = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-ExtraBold.ttf", 20, &config);


    io.FontDefault = io.Fonts->AddFontFromFileTTF("./Roboto-Regular.ttf", 20, &config);
    const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("./fontawesome-webfont.ttf", 20.0f, &config, icon_ranges);             // Merge into first font
    io.Fonts->Build();
    fonts["Default"] = io.FontDefault;
  

}
EngineAssetManager::~EngineAssetManager()
{
    // Destroy();
}

void EngineAssetManager::Destroy()
{
    Texture::DestroyTexture(textures["File Icon"]);
    Texture::DestroyTexture(textures["Folder Icon"]);
    Texture::DestroyTexture(textures["Project"]);
}