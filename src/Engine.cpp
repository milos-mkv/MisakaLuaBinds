#include <engine/Engine.hpp>
#include <engine/gl/Texture.hpp>
#include <engine/ui/EngineUI.hpp>

#pragma region ========================== INCLUDE ==========================

    #include <utils/Logger.hpp>
    #include <GLFW/glfw3.h>
    #include <OpenGL/gl3.h>
    // #include <stb_image.h>
    #include <imgui.h>
    #include <imgui_impl_glfw.h>
    #include <imgui_impl_opengl3.h>
    #include <imgui_internal.h>
    #include <TextEditor.h>
    #include <fstream>
    #include <filesystem>
    #include <FontAwesomeIcons.hpp>
    #include <engine/ui/EngineUI.hpp>

#pragma endregion

std::shared_ptr<Engine> Engine::Get()
{
    if (s_instance == nullptr)
        s_instance = std::make_shared<Engine>();
    return s_instance;
}

Engine::Engine()
{
    LOG("Engine initialize!");
    InitializeGLFW();
    InitializeImGui();
}



void Engine::Run()
{
    TextEditor editor;
    auto lang = TextEditor::LanguageDefinition::Lua();
	editor.SetLanguageDefinition(lang);

	static const char* fileToEdit = "../scripts/script.lua";
    {
		std::ifstream t(fileToEdit);
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			editor.SetText(str);
		}
	}

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

    EngineUI::Get()->font = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-ExtraBold.ttf", 20);

   

    // id1 = Texture::CreateTexture("./folder.png");
    // id2 = Texture::CreateTexture("./file.png");

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        
        // Clear default framebuffer.
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderImGui();

        glfwSwapBuffers(m_window);
    }
}

void Engine::RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();  
    ImGui::NewFrame();
    DID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    // Add UI;
    EngineUI::Get()->Render();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
}

Engine::~Engine()
{
    LOG("Engine destroy!");
    EngineUI::Get()->Destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

#pragma region ===================== Initialize Engine =====================

void Engine::InitializeGLFW()
{
    LOG("Engine::InitializeGLFW");
    if(glfwInit() != GLFW_TRUE)
    {
        LOG("Failed to initialize GLFW !");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

    m_window = glfwCreateWindow(1000, 600, "Misaka Engine", nullptr, nullptr);
    if (m_window == nullptr)
    {
        LOG("Failed to create GLFW window!");
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        LOG("GLFW Frame buffer size callback:", width, height);
        glViewport(0, 0, width, height);
    });
    // glfwSwapInterval(1);
}

void Engine::InitializeImGui()
{
    LOG("Engine::InitializeImGui");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    SetStyleForImGui();

}

void Engine::SetStyleForImGui()
{
    LOG("Engine::SetStyleForImGui");
    auto bgColor = ImVec4(0.0941,0.0941,0.0941, 1.00f);
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = bgColor;
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_TitleBg]               = bgColor;
    style.Colors[ImGuiCol_TitleBgActive]         = bgColor;
    style.Colors[ImGuiCol_TitleBgCollapsed]      = bgColor;
    style.Colors[ImGuiCol_MenuBarBg]             = bgColor;
    style.Colors[ImGuiCol_ScrollbarBg]           = bgColor;
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Separator]             = bgColor; //style.Colors[ImGuiCol_Border];
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab]                   = bgColor; //ImGui::ColorConvertU32ToFloat4(4279242768); //ImVec4(1.08f, 0.08f, 0.09f, 0.83f);
    style.Colors[ImGuiCol_TabHovered]            = ImGui::ColorConvertU32ToFloat4(4279242768);
    style.Colors[ImGuiCol_TabActive]             = ImGui::ColorConvertU32ToFloat4(4279242768); //ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]          = bgColor;
    style.Colors[ImGuiCol_TabUnfocusedActive]    = ImGui::ColorConvertU32ToFloat4(4279242768);
    style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.GrabRounding                           = style.FrameRounding = 2.3f;
    // style.WindowMenuButtonPosition               = ImGuiDir_None;
    style.TabRounding = 0;
}

#pragma endregion