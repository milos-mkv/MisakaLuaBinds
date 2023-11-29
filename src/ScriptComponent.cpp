// #include <components/ScriptComponent.hpp>

// #include <iostream>
// #include <Game.hpp>

// static bool LuaStateCheckIfOK(lua_State* L, int r)
// {
//     if (r != LUA_OK)
//     {
//         std::cout << lua_tostring(L, -1) << std::endl;
//         lua_pop(L, -1);
//         return false;
//     }
//     return true;
// }

// ScriptComponent::ScriptComponent(const char* path)
//     : m_path(path)
// {
//     m_state = luaL_newstate();
//     luaL_openlibs(m_state);

//     Game::Get()->RegisterGameModule(m_state);

//     if (LuaStateCheckIfOK(m_state, luaL_dofile(m_state, m_path.c_str())))
//     {
        
//     }
// }

// ScriptComponent::~ScriptComponent()
// {
//     lua_close(m_state);
// }