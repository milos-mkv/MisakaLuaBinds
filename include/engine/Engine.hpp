#ifndef MISAKA_ENGINE
#define MISAKA_ENGINE

#include <GLFW/glfw3.h>
#include <engine/ui/EngineUI.hpp>
#include <utils/Types.hpp>
class Engine
{
private:
    static inline PTR<Engine> s_instance = nullptr;

public:
    static PTR<Engine> Get();

    GLFWwindow* m_window;

    Engine();
   ~Engine();

    void Run();

private:
    void InitializeGLFW();
    void InitializeImGui();
    void SetStyleForImGui();
    void RenderImGui();
};

#endif