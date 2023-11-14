#ifndef MISAKA_LUA_SCRIPT_COMPONENT
#define MISAKA_LUA_SCRIPT_COMPONENT

#include <lua.hpp>
#include <string>

class ScriptComponent
{
private:
    lua_State* m_state;
    std::string m_path;

public:
    ScriptComponent(const char* path);
   ~ScriptComponent();
};

#endif