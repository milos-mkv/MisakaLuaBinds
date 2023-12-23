#include <engine/Engine.hpp>

#include <utils/Logger.hpp>
#include <OpenGL/gl3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

PTR<Engine> Engine::Get()
{
    if (s_instance == nullptr)
        s_instance = CreatePTR(Engine);
    return s_instance;
}

Engine::Engine()
{
    LOG("Engine::Engine");
    InitializeGLFW();
    InitializeImGui();
}

void Engine::Run()
{
    LOG("Engine::Run");

    EngineUI::Get();

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        
        glClearColor(0.0641, 0.0641, 0.0641, 1.00f);
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
    ImGui::SetNextWindowPos({ -1, 100});
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    EngineUI::Get()->Render();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Engine::~Engine()
{
    LOG("Engine::~Engine");
    EngineUI::Get()->Destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}


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
    glfwSwapInterval(1);
}

void Engine::InitializeImGui()
{
    LOG("Engine::InitializeImGui");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    SetStyleForImGui();
}

void Engine::SetStyleForImGui()
{
    LOG("Engine::SetStyleForImGui");
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f,1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_ChildBg]               = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_PopupBg]               = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_Border]                = ImVec4(0.3f, 0.3f, 0.3f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_TitleBg]               = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_TitleBgActive]         = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_TitleBgCollapsed]      = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_MenuBarBg]             = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_ScrollbarBg]           = BACKGROUND_COLOR;
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
    style.Colors[ImGuiCol_Separator]             = ImVec4(0.3, 0.3, 0.3, 1.0);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab]                   = BACKGROUND_COLOR; 
    style.Colors[ImGuiCol_TabHovered]            = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_TabActive]             = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_TabUnfocused]          = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_TabUnfocusedActive]    = BACKGROUND_COLOR;
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
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.00f, 0.00f, 0.00f, 0.75f);
    style.Colors[ImGuiCol_TableRowBg]            = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);

    style.TabRounding       = 0;
    style.ChildRounding     = 0;
    style.GrabRounding      = 0;
    style.FrameRounding     = 0;
    style.PopupRounding     = 0;
    style.ScrollbarRounding = 0;
    style.WindowRounding    = 0;
    style.ChildBorderSize   = 0;
    style.TabBorderSize     = 0;
}