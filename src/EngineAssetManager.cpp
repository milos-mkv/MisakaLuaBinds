#include <engine/ui/EngineAssetManager.hpp>

#include <utils/Logger.hpp>
#include <FontAwesomeIcons.hpp>

PTR<EngineAssetManager> EngineAssetManager::Get()
{
    if (s_instance == nullptr)
        s_instance = CreatePTR(EngineAssetManager);
    return s_instance;
}

EngineAssetManager::EngineAssetManager()
{
    m_textures["File Icon"]   = Texture::CreateTexture("file.png");
    m_textures["Folder Icon"] = Texture::CreateTexture("folder.png");
    m_textures["Project"]     = Texture::CreateTexture("project.png");

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig config;

    config.OversampleH = 4;
    config.OversampleV = 4;
    config.PixelSnapH = false;
    m_fonts["JetBrains"] = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-ExtraBold.ttf", 20, &config);
    m_fonts["JetBrains24"] = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-Regular.ttf", 26, &config);

    io.FontDefault = io.Fonts->AddFontFromFileTTF("./Roboto-Regular.ttf", 20, &config);
    const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    config.MergeMode = true;

    io.Fonts->AddFontFromFileTTF("./fontawesome-webfont.ttf", 20.0f, &config, icon_ranges);             // Merge into first font
    io.Fonts->Build();
    m_fonts["Default"] = io.FontDefault;

}

EngineAssetManager::~EngineAssetManager()
{
    LOG("EngineAssetManager::~EngineAssetManager");
}

void EngineAssetManager::Destroy()
{
    LOG("EngineAssetManager::Destroy");

    for(const auto& [key, value] : m_textures)
    {
        Texture::DestroyTexture(value);
    }
}