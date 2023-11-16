#ifndef MISAKA_ENGINE
#define MISAKA_ENGINE

#include <memory>
#include <GLFW/glfw3.h>

class Engine
{
private:
    static inline std::shared_ptr<Engine> s_instance = nullptr;

public:
    GLFWwindow* m_window;

    static void CreateEngine();
    static std::shared_ptr<Engine> Get() { return s_instance; }

    Engine();
   ~Engine();

    void Run();
};

#endif