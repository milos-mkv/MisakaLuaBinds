#include <Game.hpp>
#include <Utils.hpp>
#include <GLFW/glfw3.h>
#include <GameWindow.hpp>

void Game::CreateGame()
{
    if (m_instance == nullptr)
        m_instance = std::make_shared<Game>();
}

Game::Game()
{
    Log("Initialize Game!");
    glfwInit();
}

int lua_LoadModule(lua_State* state)
{
    std::string module = (const char*) luaL_checkstring(state, 1);
    lua_RegisterGameWindow(state);
    return 0;
}

void Game::RegisterGameModule(lua_State* state)
{
    static const luaL_Reg game_methods[] = {
		{ "LoadModule", lua_LoadModule },
		{ NULL,	NULL },
	};

	luaL_newmetatable(state, "Game_Methods");
	lua_pushvalue(state, -1);
	lua_setfield(state, -2, "__index");
	luaL_setfuncs(state, game_methods, 0);
	lua_setglobal(state, "Game");
}