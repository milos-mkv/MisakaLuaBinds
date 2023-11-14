#ifndef MISAKA_LUA_GAME
#define MISAKA_LUA_GAME

#include <memory>
#include <lua.hpp>

class Game
{
private:
    static inline std::shared_ptr<Game> m_instance = nullptr;

public:
    Game();

    void RegisterGameModule(lua_State* state);

    static std::shared_ptr<Game> Get() { return m_instance; }

    static void CreateGame();
};

#endif