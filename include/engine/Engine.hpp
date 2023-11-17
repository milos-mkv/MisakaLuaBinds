#ifndef MISAKA_ENGINE
#define MISAKA_ENGINE

#include <memory>
#include <GLFW/glfw3.h>
#include "ui/EngineUI.hpp"

class Engine
{
private:
    static inline std::shared_ptr<Engine> s_instance = nullptr;

public:
    GLFWwindow* m_window;

    static std::shared_ptr<Engine> Get();

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