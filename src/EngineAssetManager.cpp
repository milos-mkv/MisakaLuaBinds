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

EngineAssetManager::EngineAssetManager()
{
    textures["File Icon"] = Texture::CreateTexture("file.png");
    textures["Folder Icon"] = Texture::CreateTexture("folder.png");
    textures["Project"] = Texture::CreateTexture("project.png");


    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImFont* font = io.Fonts->AddFontDefault();
    ImFontConfig config1;
    config1.OversampleH = 4;
    config1.OversampleV = 4;
    config1.PixelSnapH = false;

    io.Fonts->AddFontFromFileTTF("./Roboto-Regular.ttf", 20, &config1);
    const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig config;
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("./fontawesome-webfont.ttf", 20.0f, &config, icon_ranges);             // Merge into first font
    io.Fonts->Build();

    fonts["JetBrains"] = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-ExtraBold.ttf", 20);

   
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