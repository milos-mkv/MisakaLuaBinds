#ifndef MISAKA_GAME_WINDOW
#define MISAKA_GAME_WINDOW

#include <string>
#include <memory>
#include <lua.hpp>
#include <GLFW/glfw3.h>
#include "Utils.hpp"

class GameWindow
{
public:
    static inline std::shared_ptr<GameWindow> s_instance = nullptr;

public:
    int m_width;
    int m_height;
    std::string m_title;
    GLFWwindow* m_window;

    static void CreateGameWindow(int width, int height, const char* title);

    GameWindow(int width, int height, const char* title);

    void Run();
};

void lua_RegisterGameWindow(lua_State* state);

#endif